#include "include/component.h"

Component::Component(ComponentType type, QPolygonF poly, TwoDeg direction)
    : QObject(), mType(type), mPoly(poly), mDirection(direction)
{
    switch (type)
    {
        case ComponentType::HeatSink:
            mTemperature = 25;
            mHeatTransfer = 3.0;
            mMass = 100;
            break;
        case ComponentType::RotateThruster:
        case ComponentType::CruiseThruster:
            mTemperature = 25;
            mHeatTransfer = 1.0;
            mMass = 100;
            break;
        case ComponentType::Reactor:
            mTemperature = 300;
            mHeatTransfer = 1.0;
            mMass = 100;
    }
}

void Component::applyTemperatureDelta(qreal deltaTemp)
{
    mTemperature += deltaTemp;
    if (mTemperature < 0) mTemperature = 0;
}