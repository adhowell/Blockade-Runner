#ifndef ENGINE_H
#define ENGINE_H
#include "include/vector.h"
#include "include/directions.h"

class Engine {
public:
    enum Profile {
        EXP,
        LIN
    };
    enum Size {
        BIG,
        SMALL
    };

    bool isForwardAcc() const;
    bool isBackwardAcc() const;
    bool isLateralLeftAcc() const;
    bool isLateralRightAcc() const;
    bool isRotateLeftAcc() const;
    bool isRotateRightAcc() const;

    qreal getLongitudinalAcc() const;
    qreal getLateralAcc() const;
    qreal getRotationalAcc() const;

    bool enabled() const { return mEnabled; }

    QPolygonF getPoly() const { return mPoly; }
    qreal getOpacity() const { return mThrustRatio; }
    void createPoly(QPointF marker);

    void incrementAccProfile();
    void decrementAccProfile();
    void updateThrustRatio();

protected:
    Engine(TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia, qreal thrust, qreal incr, Profile profile, Size size);

    qreal mForwardAcc = 0.0;
    qreal mLateralAcc = 0.0;
    qreal mRotateAcc = 0.0;

    qreal mThrust;
    qreal mThrustRatio = 0;
    qreal mThrustRatioStep = 0;
    Profile mThrustRatioFunction;
    qreal mIncr;
    TwoDeg mDirection;

    bool mEnabled = false;
    Size mSize;
    QPolygonF mPoly;
};
#endif