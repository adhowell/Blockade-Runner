#include "include/player_ship.h"
#include "include/mini_engine.h"
#include "include/cruise_engine.h"
#include "include/heat_flow.h"
#include "include/radar_sensor.h"


PlayerShip::PlayerShip(Faction faction, uint32_t uid) : WorldObject(faction, uid)
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

    mA = {0, 0};
    mRotA = 0;
    for (const auto& e : mEngines)
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
            mA += (Vector(mAtan2) * e->getLongitudinalAcc());
            mA += (Vector(mAtan2 + M_PI_2) * e->getLateralAcc());
            mRotA += e->getRotationalAcc();
        }
    }

    // Temperature flow modelling between components
    HeatFlow hf {mComponentMap};
    hf.compute();

    mRotV += mRotA * deltaT;
    mV += mA * deltaT;

    mAtan2 += mRotV * deltaT;
    while (mAtan2 > 2.0*M_PI) mAtan2 -= 2.0*M_PI;
    while (mAtan2 < 0.0) mAtan2 += 2.0*M_PI;
    mTacticalGraphicsItem->update();
}

void PlayerShip::addReactor(int x, int y)
{
    // Do not allow more than one reactor
    for (const auto& c : mComponentMap)
    {
        if (c->getType() == CT::Reactor)
        {
            handleRemovePart({c->x(), c->y()});
            break;
        }
    }
    auto component = std::make_shared<Component>(CT::Reactor, x, y);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::addHeatSink(int x, int y)
{
    auto component = std::make_shared<Component>(CT::HeatSink, x, y);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::addRotateThruster(int x, int y)
{
    auto component = std::make_shared<Component>(CT::RotateThruster,x, y);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::addCruiseThruster(int x, int y, TwoDeg direction)
{
    auto component = std::make_shared<Component>(CT::CruiseThruster, x, y, direction);
    mComponentMap[QPair{x, y}] = component;
}

void PlayerShip::addSensor(int x, int y, TwoDeg direction)
{
    auto component = std::make_shared<Component>(CT::RADAR, x, y, direction);
    mComponentMap[QPair{x, y}] = component;
}

template<class T>
void PlayerShip::computeEngineDirectionForce(int x, int y, TwoDeg direction)
{
    // The centre-of-mass offset of the thruster determines which forces it will affect
    Vector offset = Vector(qreal((x+0.5)-gGridSize*0.5)*gBlockSize,
                           qreal((y+0.5)-gGridSize*0.5)*gBlockSize)
                    - mCentreOfMass;
    auto engine = std::make_shared<T>(mComponentMap[QPair(x, y)], direction, offset, mM, mI);
    connect(engine.get(), &Engine::transmitStatus, this, &PlayerShip::receiveTextFromComponent);

    // For visualising active thrusters
    qreal scenePosX = ((x+0.5) - (gGridSize*0.5)) * gGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (gGridSize*0.5)) * gGridSize * 2.0;
    switch (direction) {
        case TwoDeg::Up:
            scenePosY += gGridSize;
            break;
        case TwoDeg::Down:
            scenePosY -= gGridSize;
            break;
        case TwoDeg::Left:
            scenePosX += gGridSize;
            break;
        case TwoDeg::Right:
            scenePosX -= gGridSize;
            break;
    }
    engine->createPoly(QPointF(scenePosX, scenePosY));
    mEngines << engine;
}

void PlayerShip::computeRotationalInertia()
{
    mI = 0;
    mI = std::accumulate(mComponentMap.cbegin(), mComponentMap.cend(), mI,
                    [this](auto inertia, auto c)
                    {
                        inertia += c->getMass()
                                   *(qPow(qreal((c->x()+0.5)-gGridSize*0.5)*gBlockSize - mCentreOfMass.x(), 2.0)
                                     + qPow(qreal((c->y()+0.5)-gGridSize*0.5)*gBlockSize - mCentreOfMass.y(), 2.0));
                        return inertia;
                    });
}

void PlayerShip::createComponentSensors(std::shared_ptr<Component>& c)
{
    bool hasValidArc = false;
    if (c->getType() != CT::RADAR)
        return;

    // TODO: Check for allowed arc
    // Get the bearing angle to every other component
    qreal minBearing;
    qreal maxBearing;

    TwoDeg direction = c->getDirection();
    if (isGridLineFree(c->x(), c->y(), direction, true))
    {
        qreal angle = 0;
        switch (direction)
        {
            case TwoDeg::Up: break;
            case TwoDeg::Right: angle = M_PI * 0.5; break;
            case TwoDeg::Down: angle = M_PI; break;
            case TwoDeg::Left: angle = M_PI * 1.5; break;
        }
        mSensors << std::make_shared<RadarSensor>(this, angle);
        hasValidArc = true;
    }
    if (!hasValidArc)
    {
        c->setValid(false);
    }
}

void PlayerShip::createAllSubComponents()
{
    computeRotationalInertia();
    for (auto c : mComponentMap)
    {
        createComponentEngines(c);
        createComponentSensors(c);
    }
}

void PlayerShip::createComponentEngines(std::shared_ptr<Component>& c)
{
    bool hasValidEngine = false;
    bool hasEngine = false;
    if (c->getType() == CT::RotateThruster)
    {
        // For each unique direction
        for (int i = 0; i < 4; i++) {
            auto direction = static_cast<TwoDeg>(i);
            if (isGridLineFree(c->x(), c->y(), direction))
            {
                computeEngineDirectionForce<MiniEngine>(c->x(), c->y(), direction);
                hasValidEngine = true;
            }
        }
        hasEngine = true;
    }
    if (c->getType() == CT::CruiseThruster)
    {
        TwoDeg direction = c->getDirection();
        if (isGridLineFree(c->x(), c->y(), direction))
        {
            computeEngineDirectionForce<CruiseEngine>(c->x(), c->y(), direction);
            hasValidEngine = true;
        }
        hasEngine = true;
    }
    if (hasEngine && !hasValidEngine)
    {
        c->setValid(false);
    }
}

void PlayerShip::computeCentreOfRotation()
{
    /**
     * This isn't really a thing, but it's a somewhat useful indicator for
     * when rotational symmetry is either off or misaligned with the CoM.
     */
    Vector leftRotate {0, 0};
    Vector rightRotate {0, 0};
    qreal leftRotateEffectiveMass;
    qreal rightRotateEffectiveMass;
    mCanRotate = false;
    for (const auto& e : mEngines)
    {
        if (e->getComponent()->getType() == CT::CruiseThruster)
            continue;
        if (e->isRotateLeftAcc())
        {
            leftRotate += Vector(qreal((e->getComponent()->x()+0.5)-(gGridSize*0.5))*gBlockSize,
                                 qreal((e->getComponent()->y()+0.5)-(gGridSize*0.5))*gBlockSize) * e->getComponent()->getMass();
            leftRotateEffectiveMass += e->getComponent()->getMass();
            mCanRotate = true;
        }
        else if (e->isRotateRightAcc())
        {
            rightRotate += Vector(qreal((e->getComponent()->x()+0.5)-(gGridSize*0.5))*gBlockSize,
                                  qreal((e->getComponent()->y()+0.5)-(gGridSize*0.5))*gBlockSize) * e->getComponent()->getMass();
            rightRotateEffectiveMass += e->getComponent()->getMass();
            mCanRotate = true;
        }
    }
    leftRotate *= 1.0/leftRotateEffectiveMass;
    rightRotate *= 1.0/rightRotateEffectiveMass;
    if (rightRotateEffectiveMass > 0 && leftRotateEffectiveMass > 0)
    {
        mCentreOfRotation = Vector(rightRotate.x() + 0.5 * (leftRotate.x() - rightRotate.x()),
                                   rightRotate.y() + 0.5 * (leftRotate.y() - rightRotate.y()));
    }
    else if (leftRotateEffectiveMass > 0)
    {
        mCentreOfRotation = leftRotate;
    }
    else if (rightRotateEffectiveMass > 0)
    {
        mCentreOfRotation = rightRotate;
    }
}

void PlayerShip::computeProperties()
{
    mM = 0;
    mCentreOfMass = Vector(0, 0);
    for (auto c : mComponentMap)
    {
        c->setValid(true);
        if (!hasPathToReactor(c->x(), c->y()))
        {
            c->setValid(false);
        }
        mCentreOfMass += Vector(qreal((c->x()+0.5)-(gGridSize*0.5))*gBlockSize,
                                qreal((c->y()+0.5)-(gGridSize*0.5))*gBlockSize) * c->getMass();
        mM += c->getMass();
    }
    mCentreOfMass *= 1.0/mM;
}

bool PlayerShip::isGridLineFree(int x, int y, TwoDeg direction, bool flip)
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
    if (flip)
    {
        deltaX *= -1;
        deltaY *= -1;
    }
    x += deltaX;
    y += deltaY;
    while (0 <= x && x <= gGridSize && 0 <= y && y <= gGridSize)
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
    while (mRotateTargetRad > 2.0*M_PI) mRotateTargetRad -= 2.0*M_PI;
    while (mRotateTargetRad < 0.0) mRotateTargetRad += 2.0*M_PI;
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
        case CT::RADAR:
            addSensor(pos.x(), pos.y(), direction);
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
    // TODO: Make this less bad
    auto playerItem = dynamic_cast<PlayerShipItem*>(mTacticalGraphicsItem);

    Q_EMIT handleRemoveAllConfigItems();
    playerItem->reset();
    for (const auto& c : mComponentMap)
    {
        Q_EMIT handleAddConfigComponent(c);
        playerItem->addComponent(c);
    }
    for (const auto& e : mEngines)
    {
        Q_EMIT handleAddConfigEngine(e);
        playerItem->addEngine(e);
    }
    Q_EMIT handleAddCentreOfMass(mCentreOfMass.x(), mCentreOfMass.y());
    if (mCanRotate)
    {
        Q_EMIT handleAddCentreOfRotation(mCentreOfRotation.x(), mCentreOfRotation.y());
    }

    Q_EMIT handleAddSensors(mSensors);
}

void PlayerShip::reconfigure()
{
    // Clear stuff
    mEngines.clear();
    Q_EMIT handleClearSensors(mSensors);
    mSensors.clear();

    // Compute physics stuff
    computeProperties();
    createAllSubComponents();
    computeCentreOfRotation();

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
    return std::any_of(checked.cbegin(), checked.cend(),
                       [this](auto c)
                       {
                           return mComponentMap[{c>>4, c&0xF}]->getType() == CT::Reactor;
                       });
}
