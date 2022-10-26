#include "sensor.h"
#include "include/world_object.h"

#pragma once


class RadarSensor : public Sensor
{
public:
    RadarSensor(WorldObject* parent, qreal boreOffset, qreal cwStart, qreal cwEnd, qreal scanFOV = M_PI*0.1, qreal scanSpeed = 0.01)
            : Sensor(Sensor::Radiation::Radio, Sensor::Radiation::Radio,
                     100, 0,
                     boreOffset,
                     cwStart,
                     cwEnd,
                     scanFOV, scanSpeed) {}
};