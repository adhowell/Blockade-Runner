#include "engine.h"

#pragma once


class MiniEngine : public Engine {
public:
    MiniEngine(std::shared_ptr<Component> component, TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia);
};

MiniEngine::MiniEngine(std::shared_ptr<Component> component, TwoDeg direction, Vector centreOfMassOffset, qreal mass, qreal inertia)
        : Engine(component, direction, centreOfMassOffset, mass, inertia, 1, 1.0, Profile::LIN, Size::SMALL) {}
