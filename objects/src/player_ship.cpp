#include "include/player_ship.h"
#include "include/mini_engine.h"
#include "include/cruise_engine.h"

PlayerShip::PlayerShip()
{
    mTacticalGraphicsItem = new PlayerShipItem(&mAtan2);
    addBridge(2, 2);
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
                Q_EMIT displayText("<INFO> - HALF-ROTATION COMPLETE");
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
                Q_EMIT displayText("<INFO> - HALF-ROTATION COMPLETE");
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
                Q_EMIT displayText("<INFO> - ROTATE COMMAND COMPLETE");
            }
            break;
        case Idle:;
    }

    // Temperature flow between components
    QMapIterator compIter(mComponentMap);
    while (compIter.hasNext())
    {
        compIter.next();
        auto c = compIter.value();
        int x = compIter.key().first;
        int y = compIter.key().second;
        if (mComponentMap.contains({x-1, y}))
            c->applyTemperatureDelta(0.01 * (mComponentMap[{x-1, y}]->getTemperature() - c->getTemperature()));
        if (mComponentMap.contains({x+1, y}))
            c->applyTemperatureDelta(0.01 * (mComponentMap[{x+1, y}]->getTemperature() - c->getTemperature()));
        if (mComponentMap.contains({x, y-1}))
            c->applyTemperatureDelta(0.01 * (mComponentMap[{x, y-1}]->getTemperature() - c->getTemperature()));
        if (mComponentMap.contains({x, y+1}))
            c->applyTemperatureDelta(0.01 * (mComponentMap[{x, y+1}]->getTemperature() - c->getTemperature()));
    }

    for (auto e : mEngines)
    {
        // Determine if the engine should be fired
        bool enable = false;
        if (mForwardThrust && e->isForwardAcc()) {
            enable = true;
        } if (mBackwardThrust && e->isBackwardAcc()) {
            enable = true;
        } if (mLeftThrust && e->isLateralLeftAcc()) {
            enable = true;
        } if (mRightThrust && e->isLateralRightAcc()) {
            enable = true;
        } if (mRotateLeftThrust && e->isRotateLeftAcc()) {
            enable = true;
        } if (mRotateRightThrust && e->isRotateRightAcc()) {
            enable = true;
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
    mAtan2 += mRotV * deltaT;
    mTacticalGraphicsItem->update();
}

void PlayerShip::addBridge(int x, int y)
{
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
            << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = new Component(Component::ComponentType::Reactor, poly);
    mComponentMap[QPair{x, y}] = component;
    mCentreOfMass += Vector(qreal((x+0.5)-(sGridSize*0.5))*sBlockSize,
                            qreal((y+0.5)-(sGridSize*0.5))*sBlockSize) * component->getMass();

    mTacticalGraphicsItem->addComponent(component);
    mM += component->getMass();
}

void PlayerShip::addCargo(int x, int y)
{
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
                             << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
                             << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
                             << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = new Component(Component::ComponentType::Cargo, poly);
    mComponentMap[QPair{x, y}] = component;
    mCentreOfMass += Vector(qreal((x+0.5)-(sGridSize*0.5))*sBlockSize,
                            qreal((y+0.5)-(sGridSize*0.5))*sBlockSize) * component->getMass();

    mTacticalGraphicsItem->addComponent(component);
    mM += component->getMass();
}

void PlayerShip::addRotateThruster(int x, int y)
{
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
            << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = new Component(Component::ComponentType::RotateThruster, poly);
    mComponentMap[QPair{x, y}] = component;
    mCentreOfMass += Vector(qreal((x+0.5)-(sGridSize*0.5))*sBlockSize,
                            qreal((y+0.5)-(sGridSize*0.5))*sBlockSize) * component->getMass();

    mTacticalGraphicsItem->addComponent(component);
    mM += component->getMass();
}

void PlayerShip::addCruiseThruster(int x, int y, TwoDeg direction)
{
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSize * 2.0;
    auto poly  = QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
            << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
            << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize);

    auto component = new Component(Component::ComponentType::CruiseThruster, poly, direction);
    mComponentMap[QPair{x, y}] = component;
    mCentreOfMass += Vector(qreal((x+0.5)-(sGridSize*0.5))*sBlockSize,
                            qreal((y+0.5)-(sGridSize*0.5))*sBlockSize) * component->getMass();

    mTacticalGraphicsItem->addComponent(component);
    mM += component->getMass();
}

void PlayerShip::computeThrusterDirectionForce(int x, int y, TwoDeg direction)
{
    // The centre-of-mass offset of the thruster determines which forces it will affect
    Vector offset = Vector(qreal((x+0.5)-sGridSize*0.5)*sBlockSize,
                            qreal((y+0.5)-sGridSize*0.5)*sBlockSize)
                    - mCentreOfMass;
    auto engine = new MiniEngine(mComponentMap[QPair(x, y)], direction, offset, mM, mI);
    connect(engine, &Engine::transmitStatus, this, &PlayerShip::receiveTextFromComponent);

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
    mTacticalGraphicsItem->addEngine(engine);
}

void PlayerShip::computeCruiseEngineDirectionForce(int x, int y, TwoDeg direction)
{
    // The centre-of-mass offset of the thruster determines which forces it will affect
    Vector offset = Vector(qreal((x+0.5)-sGridSize*0.5)*sBlockSize,
                           qreal((y+0.5)-sGridSize*0.5)*sBlockSize)
                    - mCentreOfMass;
    auto engine = new CruiseEngine(mComponentMap[QPair(x, y)], direction, offset, mM, mI);
    connect(engine, &Engine::transmitStatus, this, &PlayerShip::receiveTextFromComponent);

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
    mTacticalGraphicsItem->addEngine(engine);
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
        if (compIter.value()->getType() == Component::ComponentType::RotateThruster)
        {
            // For each unique direction
            for (int i = 0; i < 4; i++) {
                TwoDeg direction = static_cast<TwoDeg>(i);
                if (isGridLineFree(x, y, direction))
                    computeThrusterDirectionForce(x, y, direction);
            }
        }
        if (compIter.value()->getType() == Component::ComponentType::CruiseThruster)
        {
            TwoDeg direction = compIter.value()->getDirection();
            assert (isGridLineFree(x, y, direction));
            computeCruiseEngineDirectionForce(x, y, direction);
        }
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