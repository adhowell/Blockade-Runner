#include "sensor.h"
#include "../world_objects/include/world_object.h"

#pragma once


class RadarSensor : public Sensor
{
public:
    RadarSensor(WorldObject* parent, qreal boreOffsetAngle)
            : Sensor(Sensor::Radiation::Radio, Sensor::Radiation::Radio,
                     100, 0,
                     boreOffsetAngle,
                     M_PI*0.45, M_PI*0.1, 0.01) {}
};