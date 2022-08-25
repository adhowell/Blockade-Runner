#include "include/player_ship.h"
#include "include/mini_engine.h"
#include "include/cruise_engine.h"
#include "include/heat_flow.h"

PlayerShip::PlayerShip()
{
    mTacticalGraphicsItem = new PlayerShipItem(&mAtan2);
}

void PlayerShip::update(qreal deltaT)
{
    // State machine for rotation commands
    switch (mRotateState)
    {
        case RotateState::BeforeTargetCW:
            resetMovement();
            mRotateRightThrust = true;
            if (mAtan2 >= mRotateTargetRad)
            {
                mRotateLeftThrust = true;
                mRotateRightThrust = false;
                mRotateState = RotateState::AfterTargetCW;
                Q_EMIT displayText("HALF-ROTATION COMPLETE");
            }
            break;
        case RotateState::BeforeTargetCCW:
            resetMovement();
            mRotateLeftThrust = true;
            if (mAtan2 <= mRotateTargetRad)
            {
                mRotateLeftThrust = false;
                mRotateRightThrust = true;
                mRotateState = RotateState::AfterTargetCCW;
                Q_EMIT displayText("HALF-ROTATION COMPLETE");
            }
            break;
        case RotateState::AfterTargetCW:
        case RotateState::AfterTargetCCW:
            resetMovement();
            if (qAbs(mRotV) < 0.001) {
                mRotateLeftThrust = false;
                mRotateRightThrust = false;
                mRotateState = RotateState::Shutdown;
            }
            break;
        case RotateState::Shutdown:
            mRotV *= 0.8;
            if (qAbs(mRotV) < 0.0000001)
            {
                mRotV = 0;
                mRotateState = RotateState::Idle;
                Q_EMIT displayText("ROTATE COMMAND COMPLETE");
            }
            break;
        case RotateState::Idle:;
    }

    for (auto e : mEngines)
    {
        // Determine if the engine should be fired
        bool enable = false;
        switch (e->getComponent()->getType())
        {
            case CT::RotateThruster:
                if (mLeftThrust && e->isLateralLeftAcc()) enable = true;
                if (mRightThrust && e->isLateralRightAcc()) enable = true;
                if (mRotateLeftThrust && e->isRotateLeftAcc()) enable = true;
                if (mRotateRightThrust && e->isRotateRightAcc()) enable = true;
            case CT::CruiseThruster:
                if (mForwardThrust && e->isForwardAcc()) enable = true;
                if (mBackwardThrust && e->isBackwardAcc()) enable = true;
            default:;
        }

        if (enable)
            e->incrementAccProfile();
        else
            e->decrementAccProfile();

        // Update velocity if engine is firing
        if (e->enabled()) {
            mV += (Vector(mAtan2) * e->getLongitudinalAcc() * deltaT);
            mV += (Vector(mAtan2 + M_PI_2) * e->getLateralAcc() * deltaT);
            mRotV += e->getRotationalAcc() * deltaT;
        }
    }

    // Temperature flow modelling between components
    HeatFlow hf {mComponentMap};
    hf.compute();

    mAtan2 += mRotV * deltaT;
    mTacticalGraphicsItem->update();
}

void PlayerShip::addReactor(int x, int y)
{
    // Do not allow more than one reactor
    auto compIter = QMapIterator(mComponentMap);
    while (compIter.hasNext())
    {
        compIter.next();
        if (compIter.value()->getType() == CT::Reactor)
        {
            handleRemovePart({compIter.key().first, compIter.key().second});
            break;
        }
    }

    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
            << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = std::make_shared<Component>(CT::Reactor, poly);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::addHeatSink(int x, int y)
{
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
                             << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
                             << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
                             << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = std::make_shared<Component>(CT::HeatSink, poly);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::addRotateThruster(int x, int y)
{
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
            << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = std::make_shared<Component>(CT::RotateThruster, poly);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::addCruiseThruster(int x, int y, TwoDeg direction)
{
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
            << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = std::make_shared<Component>(CT::CruiseThruster, poly, direction);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::computeThrusterDirectionForce(int x, int y, TwoDeg direction)
{
    // The centre-of-mass offset of the thruster determines which forces it will affect
    Vector offset = Vector(qreal((x+0.5)-sGridSize*0.5)*sBlockSize,
                            qreal((y+0.5)-sGridSize*0.5)*sBlockSize)
                    - mCentreOfMass;
    auto engine = std::make_shared<MiniEngine>(mComponentMap[QPair(x, y)], direction, offset, mM, mI);
    connect(engine.get(), &Engine::transmitStatus, this, &PlayerShip::receiveTextFromComponent);

    // For visualising active thrusters
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    switch (direction) {
        case TwoDeg::Up:
            scenePosY += sGridSize;
            break;
        case TwoDeg::Down:
            scenePosY -= sGridSize;
            break;
        case TwoDeg::Left:
            scenePosX += sGridSize;
            break;
        case TwoDeg::Right:
            scenePosX -= sGridSize;
            break;
    }
    engine->createPoly(QPointF(scenePosX, scenePosY));
    mEngines.push_back(engine);
}

void PlayerShip::computeCruiseEngineDirectionForce(int x, int y, TwoDeg direction)
{
    // The centre-of-mass offset of the thruster determines which forces it will affect
    Vector offset = Vector(qreal((x+0.5)-sGridSize*0.5)*sBlockSize,
                           qreal((y+0.5)-sGridSize*0.5)*sBlockSize)
                    - mCentreOfMass;
    auto engine = std::make_shared<CruiseEngine>(mComponentMap[QPair(x, y)], direction, offset, mM, mI);
    connect(engine.get(), &Engine::transmitStatus, this, &PlayerShip::receiveTextFromComponent);

    // For visualising active thrusters
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    switch (direction) {
        case TwoDeg::Up:
            scenePosY += sGridSize;
            break;
        case TwoDeg::Down:
            scenePosY -= sGridSize;
            break;
        case TwoDeg::Left:
            scenePosX += sGridSize;
            break;
        case TwoDeg::Right:
            scenePosX -= sGridSize;
            break;
    }
    engine->createPoly(QPointF(scenePosX, scenePosY));
    mEngines.push_back(engine);
}

void PlayerShip::computeRotationalInertia()
{
    mCentreOfMass *= 1.0/mM;
    QMapIterator compIter(mComponentMap);
    mI = 0;
    while (compIter.hasNext())
    {
        compIter.next();
        int x = compIter.key().first;
        int y = compIter.key().second;
        mI += compIter.value()->getMass()
              * (qPow(qreal((x+0.5)-sGridSize*0.5)*sBlockSize - mCentreOfMass.x(), 2.0)
                 + qPow(qreal((y+0.5)-sGridSize*0.5)*sBlockSize - mCentreOfMass.y(), 2.0));
    }
}

void PlayerShip::computeStaticForceVectors()
{
    computeRotationalInertia();

    // Thrusters can only be used if there is free LOS to the edge of the grid
    QMapIterator compIter(mComponentMap);
    while (compIter.hasNext())
    {
        compIter.next();
        int x = compIter.key().first;
        int y = compIter.key().second;
        bool hasValidEngine = false;
        bool hasEngine = false;
        if (compIter.value()->getType() == CT::RotateThruster)
        {
            // For each unique direction
            for (int i = 0; i < 4; i++) {
                auto direction = static_cast<TwoDeg>(i);
                if (isGridLineFree(x, y, direction))
                {
                    computeThrusterDirectionForce(x, y, direction);
                    hasValidEngine = true;
                }
            }
            hasEngine = true;
        }
        if (compIter.value()->getType() == CT::CruiseThruster)
        {
            TwoDeg direction = compIter.value()->getDirection();
            if (isGridLineFree(x, y, direction))
            {
                computeCruiseEngineDirectionForce(x, y, direction);
                hasValidEngine = true;
            }
            hasEngine = true;
        }
        if (hasEngine && !hasValidEngine)
        {
            compIter.value()->setValid(false);
        }
    }
}

void PlayerShip::computeProperties()
{
    mM = 0;
    mCentreOfMass = Vector(0, 0);
    QMapIterator compIter(mComponentMap);
    while (compIter.hasNext())
    {
        compIter.next();
        auto component = compIter.value();
        auto pos = compIter.key();
        component->setValid(true);
        if (!hasPathToReactor(compIter.key().first, compIter.key().second))
        {
            component->setValid(false);
        }
        mCentreOfMass += Vector(qreal((pos.first+0.5)-(sGridSize*0.5))*sBlockSize,
                                qreal((pos.second+0.5)-(sGridSize*0.5))*sBlockSize) * component->getMass();
        mM += component->getMass();
    }
}

bool PlayerShip::isGridLineFree(int x, int y, TwoDeg direction)
{
    int deltaX;
    int deltaY;
    switch (direction) {
        case TwoDeg::Up:
            deltaX = 0;
            deltaY = 1;
            break;
        case TwoDeg::Right:
            deltaX = -1;
            deltaY = 0;
            break;
        case TwoDeg::Down:
            deltaX = 0;
            deltaY = -1;
            break;
        case TwoDeg::Left:
            deltaX = 1;
            deltaY = 0;
            break;
    }
    x += deltaX;
    y += deltaY;
    while (0 <= x && x <= sGridSize && 0 <= y && y <= sGridSize)
    {
        if (mComponentMap.contains(QPair{x, y})) return false;
        x += deltaX;
        y += deltaY;
    }
    return true;
}

void PlayerShip::resetMovement()
{
    mForwardThrust = false;
    mBackwardThrust = false;
    mLeftThrust = false;
    mRightThrust = false;
    //mRotateLeftThrust = false;
    //mRotateRightThrust = false;
}

void PlayerShip::rotate(int degrees)
{
    if (mRotateState != RotateState::Idle)
    {
        Q_EMIT displayText("<ERROR> - ROTATE COMMAND IN PROGRESS");
        return;
    }
    resetMovement();
    if (degrees > 0)
        mRotateState = RotateState::BeforeTargetCW;
    else
        mRotateState = RotateState::BeforeTargetCCW;
    mRotateTargetRad = mAtan2 + (degrees*M_PI/360.0);
}

void PlayerShip::receiveTextFromComponent(const QString &text)
{
    Q_EMIT displayText(text);
}

void PlayerShip::handleAddPart(CT compType, QPoint pos, TwoDeg direction)
{
    switch (compType)
    {
        case CT::HeatSink:
            addHeatSink(pos.x(), pos.y());
            break;
        case CT::Reactor:
            addReactor(pos.x(), pos.y());
            break;
        case CT::RotateThruster:
            addRotateThruster(pos.x(), pos.y());
            break;
        case CT::CruiseThruster:
            addCruiseThruster(pos.x(), pos.y(), direction);
            break;
        default:
            break;
    }
    reconfigure();
}

void PlayerShip::handleRemovePart(QPoint pos)
{
    QPair<int, int> pair {pos.x(), pos.y()};
    if (mComponentMap.contains({pos.x(), pos.y()}))
    {
        mComponentMap.remove(pair);
        reconfigure();
    }
}

void PlayerShip::updateVisuals()
{
    Q_EMIT handleRemoveAllConfigItems();
    mTacticalGraphicsItem->reset();
    for (auto c : mComponentMap.values())
    {
        Q_EMIT handleAddConfigComponent(c);
        mTacticalGraphicsItem->addComponent(c);
    }
    for (auto e : mEngines)
    {
        Q_EMIT handleAddConfigEngine(e);
        mTacticalGraphicsItem->addEngine(e);
    }
    Q_EMIT handleAddCentreOfMass(mCentreOfMass.x(), mCentreOfMass.y());
}

void PlayerShip::reconfigure()
{
    mEngines.clear();

    // Compute physics stuff
    computeProperties();
    computeStaticForceVectors();

    updateVisuals();
}

bool PlayerShip::hasPathToReactor(int x, int y)
{
    QSet<int> checked {x<<4|y};
    while (true)
    {
        int size = checked.size();
        for (auto c : checked)
        {
            int xc {c>>4};
            int yc {c&0xF};
            if (mComponentMap.contains({xc-1, yc})) checked << ((xc-1)<<4|yc);
            if (mComponentMap.contains({xc+1, yc})) checked << ((xc+1)<<4|yc);
            if (mComponentMap.contains({xc, yc-1})) checked << (xc<<4|(yc-1));
            if (mComponentMap.contains({xc, yc+1})) checked << (xc<<4|(yc+1));
        }
        if (checked.size() == size) break;
    }
    for (auto c : checked)
    {
        if (mComponentMap[{c>>4, c&0xF}]->getType() == CT::Reactor)
            return true;
    }
    return false;
}
