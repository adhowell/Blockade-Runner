#include "include/player_ship.h"
#include "include/mini_engine.h"
#include "include/cruise_engine.h"

QMap<PlayerShip::Component, qreal> PlayerShip::sComponentMass = { {PlayerShip::Component::Thruster, 100},
                                                                  {PlayerShip::Component::Bridge, 200},
                                                                  {PlayerShip::Component::CruiseThruster, 200}};

PlayerShip::PlayerShip()
{
    addBridge(2, 2);
}

void PlayerShip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    QBrush fillBrush;
    QBrush emptyBrush;

    pen.setColor(QColor(0, 255, 0));
    fillBrush.setStyle(Qt::BrushStyle::SolidPattern);
    fillBrush.setColor(QColor(0, 255, 0));

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->rotate(mAtan2*360.0/(M_PI*2.0));

    for (auto const &p : mPolys)
    {
        painter->drawPolygon(p);
    }
    for (auto const &e : mEngines)
    {
        painter->setBrush(emptyBrush);
        fillBrush.setColor(QColor(0, 255, 0, e->getOpacity()*255));
        if (e->enabled()) painter->setBrush(fillBrush);
        painter->drawPolygon(e->getPoly());
    }
    //painter->rotate(-mAtan2*360.0/(M_PI*2.0));
    /*
    QPolygonF poly = QPolygonF() << QPointF(qCos(mAtan2)*50.0, qSin(mAtan2)*50.0)
                                 << QPointF(qCos(mAtan2 + M_PI - 0.5)*50.0, qSin(mAtan2 + M_PI - 0.5)*50.0)
                                 << QPointF(qCos(mAtan2 + M_PI)*25.0, qSin(mAtan2 + M_PI)*25.0)
                                 << QPointF(qCos(mAtan2 + M_PI + 0.5)*50.0, qSin(mAtan2 + M_PI + 0.5)*50.0);
    painter->drawPolygon(poly);
    */
    painter->rotate(-mAtan2*360.0/(M_PI*2.0));
    //painter->drawLine(0, 0, mV.x()*10, -mV.y()*10);
}

QRectF PlayerShip::boundingRect() const
{
    // Making the bounding rect too big helps with rendering fast moving PlayerShips
    return QRectF(-50, -50, 100, 100);
}

void PlayerShip::update(qreal deltaT)
{
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
    prepareGeometryChange();
}

void PlayerShip::velocityMultiply(qreal scalar)
{
    mV *= scalar;
}

void PlayerShip::velocityAddition(qreal scalar)
{
    mV += scalar;
}

void PlayerShip::addAccelerationVector(Vector a)
{
    mA += a;
}

void PlayerShip::setAccelerationVector(Vector a)
{
    mA = a;
}

void PlayerShip::addBridge(int x, int y)
{
    mComponentMap[QPair{x, y}] = Component::Bridge;
    mCentreOfMass += Vector(qreal((x+0.5)-(sGridSize*0.5))*sBlockSize,
                            qreal((y+0.5)-(sGridSize*0.5))*sBlockSize) * sComponentMass[Component::Bridge];
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    mPolys.push_back(QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
                                    << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
                                    << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
                                    << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize));
    mM += sComponentMass[Component::Bridge];
}

void PlayerShip::addThruster(int x, int y)
{
    mComponentMap[QPair{x, y}] = Component::Thruster;
    mCentreOfMass += Vector(qreal((x+0.5)-(sGridSize*0.5))*sBlockSize,
                            qreal((y+0.5)-(sGridSize*0.5))*sBlockSize) * sComponentMass[Component::Thruster];
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    mPolys.push_back(QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
                                    << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
                                    << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
                                    << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize));
    mM += sComponentMass[Component::Thruster];
}

void PlayerShip::addCruiseEngine(int x, int y, TwoDeg direction)
{
    mComponentMap[QPair{x, y}] = Component::CruiseThruster;
    mComponentDirection[QPair{x, y}] = direction;
    mCentreOfMass += Vector(qreal((x+0.5)-(sGridSize*0.5))*sBlockSize,
                            qreal((y+0.5)-(sGridSize*0.5))*sBlockSize) * sComponentMass[Component::CruiseThruster];
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    mPolys.push_back(QPolygonF() << QPointF(scenePosX - sGridSceneSize, scenePosY - sGridSceneSize)
                                 << QPointF(scenePosX + sGridSceneSize, scenePosY - sGridSceneSize)
                                 << QPointF(scenePosX + sGridSceneSize, scenePosY + sGridSceneSize)
                                 << QPointF(scenePosX - sGridSceneSize, scenePosY + sGridSceneSize));
    mM += sComponentMass[Component::CruiseThruster];
}

void PlayerShip::computeThrusterDirectionForce(int x, int y, TwoDeg direction)
{
    // The centre-of-mass offset of the thruster determines which forces it will affect
    Vector offset = Vector(qreal((x+0.5)-sGridSize*0.5)*sBlockSize,
                            qreal((y+0.5)-sGridSize*0.5)*sBlockSize)
                    - mCentreOfMass;
    auto engine = new MiniEngine(direction, offset, mM, mI);

    // For visualising active thrusters
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    switch (direction) {
        case TwoDeg::Up:
            scenePosY += sGridSceneSize;
            break;
        case TwoDeg::Down:
            scenePosY -= sGridSceneSize;
            break;
        case TwoDeg::Left:
            scenePosX += sGridSceneSize;
            break;
        case TwoDeg::Right:
            scenePosX -= sGridSceneSize;
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
    auto engine = new CruiseEngine(direction, offset, mM, mI);

    // For visualising active thrusters
    qreal scenePosX = ((x+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    qreal scenePosY = ((y+0.5) - (sGridSize*0.5)) * sGridSceneSize * 2.0;
    switch (direction) {
        case TwoDeg::Up:
            scenePosY += sGridSceneSize;
            break;
        case TwoDeg::Down:
            scenePosY -= sGridSceneSize;
            break;
        case TwoDeg::Left:
            scenePosX += sGridSceneSize;
            break;
        case TwoDeg::Right:
            scenePosX -= sGridSceneSize;
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
        mI += sComponentMass[compIter.value()]
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
        if (compIter.value() == Component::Thruster)
        {
            for (int i = 0; i < 4; i++) {
                TwoDeg direction = static_cast<TwoDeg>(i);
                if (isGridLineFree(x, y, direction))
                    computeThrusterDirectionForce(x, y, direction);
            }
        }
        if (compIter.value() == Component::CruiseThruster)
        {
            TwoDeg direction = mComponentDirection[QPair(x, y)];
            assert (isGridLineFree(x, y, direction));
            computeCruiseEngineDirectionForce(x, y, direction);
        }
    }
}

bool PlayerShip::isGridLineFree(int x, int y, TwoDeg direction)
{
    int deltaX;
    int deltaY;
    int initX = x;
    int initY = y;
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
    //qDebug() << "Thruster created at X" << initX << "Y" << initY << "Direction" << direction;
    return true;
}

void PlayerShip::resetMovement()
{
    mForwardThrust = false;
    mBackwardThrust = false;
    mLeftThrust = false;
    mRightThrust = false;
    mRotateLeftThrust = false;
    mRotateRightThrust = false;
    mStrafeMode = false;
}