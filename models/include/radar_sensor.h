#include "sensor.h"
#include "include/world_object.h"

#pragma once


class RadarSensor : public Sensor
{
public:
    RadarSensor(WorldObject* parent, qreal boreOffset, qreal cwStart, qreal cwEnd)
            : Sensor(Sensor::Radiation::Radio, Sensor::Radiation::Radio,
                     100, 0,
                     boreOffset,
                     cwStart,
                     cwEnd,
                     M_PI*0.1, 0.01) {}
};