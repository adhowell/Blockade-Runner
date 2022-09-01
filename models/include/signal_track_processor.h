#include <QtWidgets>

#include "../world_objects/include/world_object.h"
#include "include/sensor.h"

#pragma once

/**
 *
 */
class SignalTrackProcessor
{
public:
    friend class WorldObject;

    explicit SignalTrackProcessor(WorldObject* parent) : mParent(parent) {}
    ~SignalTrackProcessor() = default;

    /**
     * A track is a container for the information a sensor knows
     * about a particular object in the world.
     */
    struct Track
    {
        qreal bearingOffBore;
        qreal distance;
        qreal receivedPower;
        bool factionKnown;
        uint32_t targetUid;
    };

    /**
     * Returns all of the valid tracks of the given objects.
     * @param worldObjects
     * @return vector of track objects
     */
    QVector<Track> getTracks(const QVector<WorldObject*>& worldObjects);

    /**
     * Returns a track for the given world object.
     * @param obj - The world object to check for a valid track
     * @return optional, Track object if valid, null otherwise.
     */
    std::optional<Track> computeTrack(WorldObject* obj);

    /**
     * Updates all the sensors assigned to the parent world object.
     */
    void updateSensors();

private:
    WorldObject* mParent;
};