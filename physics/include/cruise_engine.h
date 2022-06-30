#ifndef CRUISE_ENGINE_H
#define CRUISE_ENGINE_H
#include "engine.h"

#pragma once

class CruiseEngine : public Engine {
public:
    CruiseEngine(Component* component, TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia);
};

CruiseEngine::CruiseEngine(Component* component, TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia)
        : Engine(component, direction, centreOfMassOffset, mass, inertia, 15, 0.01, Profile::EXP, Size::BIG) {}

#endif //CRUISE_ENGINE_H
