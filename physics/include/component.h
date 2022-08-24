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
    };

    Component(ComponentType type, QPolygonF poly, TwoDeg direction = TwoDeg::Up);

    qreal getNormTemperature() const { return qMin((mTemperature/1000.0)+0.1, 1.0); }
    qreal getHeatInRatio() const { return mHeatInRatio; }
    qreal getHeatOutRatio() const { return mHeatOutRatio; }
    qreal getMass() const { return mMass; }
    qreal getTemperature() const { return mTemperature; }
    ComponentType getType() const { return mType; }
    TwoDeg getDirection() const { return mDirection; }
    QPolygonF getPoly() const { return mPoly; }
    bool isValid() { return mIsValid; }

    void setValid(bool isValid) { mIsValid = isValid; }

    void applyTemperatureDelta(qreal deltaTemp);

private:
    ComponentType mType;
    TwoDeg mDirection;
    qreal mTemperature;
    qreal mMass;
    QPolygonF mPoly;
    qreal mHeatInRatio;
    qreal mHeatOutRatio;
    bool mIsValid = true;
};