#ifndef ENGINE_H
#define ENGINE_H
#include "include/vector.h"
#include "include/directions.h"
#include "include/component.h"
#include <QtWidgets>

#pragma once

class Engine : public QObject {
    Q_OBJECT
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

    Component* getComponent() { return mComponent; }

    bool enabled() const { return mEnabled; }

    QPolygonF getPoly() const { return mPoly; }
    qreal getOpacity() const { return mThrustRatio; }
    qreal getNormTemperature() const { return qMin((mTemperature/1000.0)+0.1, 1.0); }
    void createPoly(QPointF marker);

    void incrementAccProfile();
    void decrementAccProfile();
    void updateThrustRatio();

Q_SIGNALS:
    void transmitStatus(const QString&);

protected:
    Engine(Component* component, TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia, qreal thrust, qreal incr, Profile profile, Size size);

    qreal mForwardAcc = 0.0;
    qreal mLateralAcc = 0.0;
    qreal mRotateAcc = 0.0;

    qreal mThrust;
    qreal mThrustRatio = 0;
    qreal mThrustRatioStep = 0;
    Profile mThrustRatioFunction;
    qreal mIncr;
    TwoDeg mDirection;
    Component* mComponent;
    qreal mTemperature;

    bool mEnabled = false;
    Size mSize;
    QPolygonF mPoly;
};
#endif