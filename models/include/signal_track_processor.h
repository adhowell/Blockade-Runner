#include "include/world_object.h"
#include "include/sensor.h"
#include "include/globals.h"

#include <QtWidgets>

#pragma once


/**
 * For handling how objects are detected by the senors aboard each platform
 */
class SignalTrackProcessor
{
public:
    explicit SignalTrackProcessor(WorldObject* parent, QVector<WorldObject*>* worldObjects) : mParent(parent), mWorldObjects(worldObjects) {}
    ~SignalTrackProcessor() = default;

    /**
     * A track is a container for the information a sensor knows
     * about a particular object in the world.
     */
    struct Track
    {
        Vector position {0, 0};
        qreal receivedPower {0};
        uint32_t timestamp {0};
    };

    /**
     * A processed track is a container for the information the processor
     * has inferred about an object in the world by accumulating tracks.
     */
    struct ProcessedTrack
    {
        uint32_t uid;
        Vector* parentPos;
        Vector offset {0, 0};
        Vector acc {0, 0};
        Vector vel {0, 0};
        Faction faction {Faction::Unknown};
        Track tracks[10];
        Track* trackStart;
        Track* trackPtr {tracks};
        Track* trackEnd;

        //ProcessedTrack(uint32_t uid, Vector* parentPos) : uid(uid), parentPos(parentPos)
        //{
        //    offset = Vector(0, 0);
        //}

        void insertTrack(Track track)
        {
            //incrementTrack();
            offset = track.position - *parentPos;
            *trackPtr = track;
        }

        void incrementTrack()
        {
            if (trackPtr == trackEnd) {
                trackPtr = trackStart;
            } else {
                trackPtr++;
            }
        }

        void decrementTrack()
        {
            if (trackPtr == trackStart) {
                trackPtr = trackEnd;
            } else {
                trackPtr--;
            }
        }

        uint32_t getLastTimestamp() const
        {
            return (*trackPtr).timestamp;
        }
    };

    /**
     * Returns all of the valid tracks of all the world objects.
     * @return vector of track objects
     */
    QVector<ProcessedTrack> getTracks();

    /**
     * Updates the tracks for all the world objects visible to the platform.
     */
    void computeTracks();

    /**
     * Returns a track for the given world object.
     * @param obj - The world object to check for a valid track
     * @return optional, Track object if valid, null otherwise.
     */
    void computeTrack(WorldObject* obj);

    WorldObject* getParent() const { return mParent; }

protected:
    WorldObject* mParent;
    QVector<WorldObject*>* mWorldObjects;
    QMap<uint32_t, ProcessedTrack> mProcessedTracks;
};