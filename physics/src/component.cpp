#include "include/component.h"

Component::Component(ComponentType type, QPolygonF poly, TwoDeg direction)
    : QObject(), mType(type), mPoly(poly), mDirection(direction)
{
    switch (type)
    {
        case ComponentType::Cargo:
        case ComponentType::RotateThruster:
        case ComponentType::CruiseThruster:
        case ComponentType::EmergencyThruster:
            mTemperature = 25;
            mMass = 100;
            break;
        case ComponentType::Reactor:
            mTemperature = 300;
            mMass = 100;
    }
}

void Component::applyTemperatureDelta(qreal deltaTemp)
{
    mTemperature += deltaTemp;
    if (mTemperature < 0) mTemperature = 0;
}