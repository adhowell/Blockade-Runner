#include "include/world_object.h"
#include "include/sensor.h"
#include "include/globals.h"

#include <QtWidgets>

#pragma once

#define MAX_TRACKS 10


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
        Track tracks[MAX_TRACKS];
        int index {0};
        bool isCurrent {false};

        void insertTrack(Track track)
        {
            isCurrent = true;
            offset = track.position - *parentPos;
            subtractOldDelta();
            tracks[index] = track;
            updateProfile();
            incrementTrack(1);
        }

        void updateProfile()
        {
            // Add the velocity for the latest track-delta
            auto oldTrack = getOffsetTrack(-1);
            if (oldTrack.timestamp == 0) {
                return;
            }
            auto delta = (tracks[index].position - oldTrack.position) * (1.0f / double(tracks[index].timestamp - oldTrack.timestamp));
            vel += delta * (1.0f / MAX_TRACKS);
        }

        void subtractOldDelta()
        {
            if (tracks[index].timestamp == 0) {
                return;
            }
            auto plusOneTrack = getOffsetTrack(1);
            if (plusOneTrack.timestamp == 0) {
                return;
            }
            auto delta = (plusOneTrack.position - tracks[index].position) * (1.0f / double(plusOneTrack.timestamp - tracks[index].timestamp));
            vel -= delta * (1.0f / MAX_TRACKS);
        }

        Track getOffsetTrack(int o)
        {
            incrementTrack(o);
            auto offsetTrack = tracks[index];
            incrementTrack(-o);
            return offsetTrack;
        }

        void incrementTrack(int o)
        {
            index += o;
            if (index < 0) {
                index += MAX_TRACKS;
            } else {
                index %= MAX_TRACKS;
            }
        }

        uint32_t getLastTimestamp() const
        {
            return tracks[index].timestamp;
        }

        void predictCurrentPosition()
        {
            isCurrent = false;
            offset += vel;
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