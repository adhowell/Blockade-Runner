#include "include/bearing.h"


Bearing::Bearing(qreal angle)
{
    mAngle = angle;
    correct();
}

qreal Bearing::operator+=(qreal delta)
{
    mAngle += delta;
    correct();
    return mAngle;
}

qreal Bearing::operator+=(const Bearing& other)
{
    mAngle += other.mAngle;
    correct();
    return mAngle;
}

qreal Bearing::operator-=(qreal delta)
{
    mAngle -= delta;
    correct();
    return mAngle;
}

qreal Bearing::operator-=(const Bearing& other)
{
    mAngle -= other.mAngle;
    correct();
    return mAngle;
}

Bearing Bearing::operator+(const Bearing& other) const
{
    return Bearing(mAngle + other.mAngle);
}

Bearing Bearing::operator-(const Bearing& other) const
{
    return Bearing(mAngle - other.mAngle);
}

bool Bearing::withinArc(qreal cwStart, qreal cwEnd)
{
    cwStart = correct(cwStart);
    cwEnd = correct(cwEnd);
    if (cwStart > cwEnd) {
        return mAngle >= cwEnd && mAngle <= cwStart;
    }
    return mAngle <= cwEnd && mAngle >= cwStart;
}

qreal Bearing::getDelta(qreal angle)
{
    qreal delta = angle - mAngle;
    if (delta > M_PI) return -m2Pi + delta;
    if (delta < -M_PI) return m2Pi + delta;
    return delta;
}

qreal Bearing::getDelta(const Bearing& other)
{
    qreal delta = other.mAngle - mAngle;
    if (delta > M_PI) return -m2Pi + delta;
    if (delta < -M_PI) return m2Pi + delta;
    return delta;
}

void Bearing::correct()
{
    while (mAngle > m2Pi) mAngle -= m2Pi;
    while (mAngle < 0) mAngle += m2Pi;
}

qreal Bearing::correct(qreal angle)
{
    while (angle > m2Pi) angle -= m2Pi;
    while (angle < 0) angle += m2Pi;
    return angle;
}