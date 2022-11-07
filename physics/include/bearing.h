#include <QtMath>

#pragma once


class Bearing
{
public:
    explicit Bearing(qreal angle);
    ~Bearing() = default;

    //T get() const { return mAngle; }
    qreal operator()() const { return mAngle; }

    Bearing& operator=(const qreal& angle) { mAngle = correct(angle); return *this; }
    Bearing& operator=(const Bearing& other) { mAngle = other.mAngle; return *this; }

    qreal operator+(const qreal& rhs) { return correct(mAngle + rhs); }
    qreal operator+=(qreal delta);
    qreal operator+=(const Bearing& other);

    qreal operator-(const qreal& rhs) { return correct(mAngle - rhs); }
    qreal operator-=(qreal delta);
    qreal operator-=(const Bearing& other);

    Bearing operator+(const Bearing& other) const;
    Bearing operator-(const Bearing& other) const;

    bool operator>(const Bearing& rhs) { return mAngle > rhs.mAngle; }
    bool operator>(qreal rhs) { return mAngle > correct(rhs); }

    bool operator<(const Bearing& rhs) { return mAngle < rhs.mAngle; }
    bool operator<(qreal rhs) { return mAngle < correct(rhs); }

    /**
     * Returns true if the current angle is within the given arc.
     */
    bool withinArc(qreal cwStart, qreal cwEnd);

    /**
     * Returns the smallest relative angle (i.e. +/- ve) to the given angle.
     */
    qreal getDelta(qreal angle);

    /**
     * Returns the smallest relative angle (i.e. +/- ve) to the angle of the given bearing object.
     */
    qreal getDelta(const Bearing& other);

private:
    qreal mAngle;
    qreal m2Pi = 2.0*M_PI;

    void correct();

    qreal correct(qreal angle);
    qreal correct(const qreal& lhs, const qreal& rhs) { return correct(lhs + rhs); }
};
