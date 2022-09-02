#include "include/world_object.h"
#include "include/sensor.h"

#include <QtWidgets>

#pragma once


/**
 * For handling how objects are detected by the senors aboard each platform
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
        Vector offset;
        qreal receivedPower;
        Faction faction;
        uint32_t uid;
    };

    /**
     * Returns all of the valid tracks of the given menu_items.
     * @param worldObjects
     * @return vector of track menu_items
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

    WorldObject* getParent() const { return mParent; }

private:
    WorldObject* mParent;
};