#include "include/player_ship.h"
#include "include/mini_engine.h"
#include "include/cruise_engine.h"
#include "include/heat_flow.h"
#include "include/radar_sensor.h"


PlayerShip::PlayerShip(Faction faction, uint32_t uid) : WorldObject(faction, uid)
{
    mTacticalGraphicsItem = new PlayerShipItem(mAtan2);
}

void PlayerShip::update(qreal deltaT)
{
    switch (mRotationController.getDirection())
    {
        case RotationController::OneDeg::Left:
            resetMovement();
            mRotateLeftThrust = true;
            mRotateRightThrust = false;
            break;
        case RotationController::OneDeg::Right:
            resetMovement();
            mRotateLeftThrust = false;
            mRotateRightThrust = true;
            break;
        default:
            mRotateLeftThrust = false;
            mRotateRightThrust = false;
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
    if (c->getType() != CT::RADAR)
        return;

    TwoDeg direction = c->getDirection();
    c->setValid(false);
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
        QPair<qreal, qreal> sensorLimits = getSensorLimits(c, angle);
        mSensors << std::make_shared<RadarSensor>(this, angle, sensorLimits.first, sensorLimits.second);
        c->setValid(true);
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
    mMaxRightRotateAcc = 0;
    mMaxLeftRotateAcc = 0;
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
            mMaxLeftRotateAcc += -e->getMaxRotationalAcc();
        }
        else if (e->isRotateRightAcc())
        {
            rightRotate += Vector(qreal((e->getComponent()->x()+0.5)-(gGridSize*0.5))*gBlockSize,
                                  qreal((e->getComponent()->y()+0.5)-(gGridSize*0.5))*gBlockSize) * e->getComponent()->getMass();
            rightRotateEffectiveMass += e->getComponent()->getMass();
            mCanRotate = true;
            mMaxRightRotateAcc += e->getMaxRotationalAcc();
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

QPair<qreal, qreal> PlayerShip::getSensorLimits(std::shared_ptr<Component> owner, qreal boreAngle)
{
    qreal minAngle = -0.75*M_PI;
    qreal maxAngle = 0.75*M_PI;
    for (const auto& c : mComponentMap)
    {
        if (c == owner) {
            continue;
        }
        auto offset = Vector(qreal(c->x()+0.5)-qreal(owner->x()+0.5), qreal(owner->y()+0.5)-qreal(c->y()+0.5));
        auto delta = Bearing(boreAngle).getDelta(offset.getAtan2());
        if (delta < 0 && delta > minAngle) minAngle = delta;
        if (delta > 0 && delta < maxAngle) maxAngle = delta;
    }
    return {-minAngle, maxAngle};
}
