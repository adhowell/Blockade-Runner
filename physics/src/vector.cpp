#include "include/vector.h"
#include <QtMath>

NormVector::NormVector(qreal dx, qreal dy)
{
    mAtan2 = qAtan2(dy, dx);
}

Vector::Vector(QLineF l, qreal vel)
{
    mSize = qAbs(vel);
    mAtan2 = qAtan2(l.x2()-l.x1(), l.y2()-l.y1()); // Transposed
    mY = qCos(mAtan2) * vel;
    mX = qSin(mAtan2) * vel;
}

Vector::Vector(qreal dx, qreal dy)
{
    mSize = qSqrt(qPow(dx, 2.0) + qPow(dy, 2.0));
    mAtan2 = qAtan2(dx, dy); // Transposed
    mX = dx;
    mY = dy;
}

Vector::Vector(qreal rad)
{
    mSize = 1.0;
    mAtan2 = rad;
    mY = qCos(mAtan2);
    mX = qSin(mAtan2);
}

void Vector::reflectAbout(qreal rad)
{
    qreal otherX = qCos(rad);
    qreal otherY = qSin(rad);
    qreal dot = mX*otherX + mY*otherY;
    mX -= 2.0*dot*otherX;
    mY -= 2.0*dot*otherY;
    mAtan2 = qAtan2(mY, mX);
}

void Vector::setSize(qreal scalar)
{
    mSize = scalar;
    mX = qSin(mAtan2) * mSize;
    mY = qCos(mAtan2) * mSize;
}

qreal Vector::separationAngle(Vector v) const
{
    qreal sep = v.mAtan2 - mAtan2 - M_PI;
    //if (v.mAtan2 < 0) sep += M_PI * 2.0;
    //if (mAtan2 < 0) sep -= M_PI * 2.0;
    return sep;
}

void Vector::reset()
{
    mX = 0.0;
    mY = 0.0;
    mAtan2 = 0.0;
    mSize = 0.0;
}

void Vector::operator+=(Vector vec)
{
    mX += vec.mX;
    mY += vec.mY;
    mAtan2 = qAtan2(mX, mY); // Transposed
    mSize = qSqrt(qPow(mX, 2.0) + qPow(mY, 2.0));
}

void Vector::operator+=(qreal scalar)
{
    mSize += scalar;
    mY = qCos(mAtan2) * mSize;
    mX = qSin(mAtan2) * mSize;
}

Vector Vector::operator+(Vector vec) const
{
    return {mX + vec.mX, mY + vec.mY};
}

void Vector::operator-=(Vector vec)
{
    mX -= vec.mX;
    mY -= vec.mY;
    mAtan2 = qAtan2(mX, mY); // Transposed
    mSize = qSqrt(qPow(mX, 2.0) + qPow(mY, 2.0));
}

Vector Vector::operator-(Vector vec) const
{
    return {mX - vec.mX, mY - vec.mY};
}

qreal Vector::operator*(Vector vec) const
{
    return mX*vec.mX + mY*vec.mY;
}

Vector Vector::operator*(qreal scalar) const
{
    return {mX*scalar, mY*scalar};
}

void Vector::operator*=(qreal scalar)
{
    mSize *= qAbs(scalar);
    if (scalar < 0) mAtan2 += M_PI;
    mY = qCos(mAtan2) * mSize;
    mX = qSin(mAtan2) * mSize;
}