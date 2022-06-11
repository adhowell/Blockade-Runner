#include "include/asteroid.h"

qreal Asteroid::sMinRadius = 5.0;
qreal Asteroid::sMaxRadius = 15.0;
qreal Asteroid::sMinMass = 1.0;
qreal Asteroid::sMaxMass = 10.0;

Asteroid::Asteroid(QColor color, qreal x, qreal y, Vector v, qreal mass, qreal radius)
        : mColor(color), mP(QPointF(x, y)), mV(v), mM(mass), mR(radius), mA(Vector(0, 0))
{
    setPos(x-mR*0.5, y-mR*0.5);
}

Asteroid::Asteroid(QColor color, QPointF p, Vector v, qreal mass, qreal radius)
        : mColor(color), mP(p), mV(v), mM(mass), mR(radius), mA(Vector(0, 0))
{
    setPos(p.x()-mR*0.5, p.y()-mR*0.5);
}

void Asteroid::posUpdate(qreal dist, qreal rad)
{
    mP += QPointF(dist * qCos(rad), dist * qSin(rad));
}

void Asteroid::posUpdate(QPointF posDelta)
{
    mP += posDelta;
}

void Asteroid::vectorReflect(qreal rad)
{
    mV.reflectAbout(rad);
}

void Asteroid::advance(qreal deltaT)
{
    // Update the velocity vector with the acceleration vector
    mV += mA * deltaT;

    // Compute new position based on delta-t
    if (mV.getSize() > 50.0) mV.setSize(50.0);
    if (mV.getSize() > 0) {
        //mV.velocityDragAdjust();
        mP += mV.getPosDelta(deltaT);
    }
}

void Asteroid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    pen.setColor(mColor);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);

    painter->drawEllipse(QRectF(-mR, -mR, mR*2.0, mR*2.0));
}

QRectF Asteroid::boundingRect() const
{
    // Making the bounding rect too big helps with rendering fast moving Asteroids
    return QRectF(-mR*2, -mR*2, mR*4.0, mR*4.0);
}

qreal Asteroid::distance(Asteroid* b) const
{
    return qSqrt(qPow(mP.x()-b->mP.x(), 2.0) + qPow(mP.y()-b->mP.y(), 2.0));
}

qreal Asteroid::atan2(Asteroid* b) const
{
    return qAtan2(mP.y()-b->mP.y(), mP.x()-b->mP.x());
};

void Asteroid::collide(Asteroid* b)
{
    Vector x1 = Vector(mP.x(), mP.y());
    Vector x2 = Vector(b->mP.x(), b->mP.y());
    Vector x12 = x1 - x2;
    Vector x21 = x2 - x1;
    Vector thisNewVec = mV - x12 * ( ((mV - b->mV) * (x12)) / qPow(x12.getSize(), 2.0) )
                             * (2.0 * b->mM / (mM + b->mM));
    Vector otherNewVec = b->mV - x21 * ( ((b->mV - mV) * (x21)) / qPow(x21.getSize(), 2.0) )
                                 * (2.0 * mM / (mM + b->mM));
    mV = thisNewVec;
    b->mV = otherNewVec;
}

void Asteroid::addVelocityVector(Vector v)
{
    mV += v;
}

void Asteroid::velocityMultiply(qreal scalar)
{
    mV *= scalar;
}

void Asteroid::velocityAddition(qreal scalar)
{
    mV += scalar;
}

void Asteroid::addAccelerationVector(Vector a)
{
    mA += a;
}

void Asteroid::setAccelerationVector(Vector a)
{
    mA = a;
}