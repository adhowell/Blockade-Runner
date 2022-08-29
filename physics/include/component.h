#include "include/directions.h"
#include "../views/include/globals.h"

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
        RADAR
    };

    Component(ComponentType type, int x, int y, TwoDeg direction = TwoDeg::Up);

    qreal getNormTemperature() const { return qMin((mTemperature/1000.0)+0.1, 1.0); }
    qreal getHeatInRatio() const { return mHeatInRatio; }
    qreal getHeatOutRatio() const { return mHeatOutRatio; }
    qreal getMass() const { return mMass; }
    qreal getTemperature() const { return mTemperature; }
    ComponentType getType() const { return mType; }
    TwoDeg getDirection() const { return mDirection; }
    QPolygonF getPoly() const { return mPoly; }
    QPolygonF getTexturePoly() const { return mTexturePoly; }
    bool isValid() { return mIsValid; }
    int x() const { return mX; }
    int y() const { return mY; }

    void setValid(bool isValid) { mIsValid = isValid; }

    void applyTemperatureDelta(qreal deltaTemp);

private:
    int mX;
    int mY;
    ComponentType mType;
    TwoDeg mDirection;
    qreal mTemperature;
    qreal mMass;
    QPolygonF mPoly;
    QPolygonF mTexturePoly;
    qreal mHeatInRatio;
    qreal mHeatOutRatio;
    bool mIsValid = true;
};