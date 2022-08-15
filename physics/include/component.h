#include "include/directions.h"

#include <QtWidgets>

#pragma once

class Component : public QObject
{
    Q_OBJECT
public:
    enum ComponentType
    {
        Reactor,
        HeatSink,
        RotateThruster,
        CruiseThruster,
        EmergencyThruster,
    };

    Component(ComponentType type, QPolygonF poly, TwoDeg direction = TwoDeg::Up);

    qreal getNormTemperature() const { return qMin((mTemperature/1000.0)+0.1, 1.0); }
    void applyTemperatureDelta(qreal deltaTemp);

    qreal getMass() const { return mMass; }
    qreal getTemperature() const { return mTemperature; }
    ComponentType getType() const { return mType; }
    TwoDeg getDirection() const { return mDirection; }
    QPolygonF getPoly() const { return mPoly; }

private:
    ComponentType mType;
    TwoDeg mDirection;
    qreal mTemperature;
    qreal mMass;
    QPolygonF mPoly;
};