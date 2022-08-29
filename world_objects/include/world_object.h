#include <QtWidgets>
#include "../physics/include/vector.h"

#pragma once

/**
 * Class that every world object must inherit from. I.e. Anything that moves in
 * the world space.
 */
class WorldObject : public QWidget
{
    Q_OBJECT
public:
    enum class Faction
    {
        Blue, // Player
        Green, // Civilians
        Red, // Hostile
        Unknown
    };

    /**
     * Initialises a new object in the world.
     *
     * @param faction - Blue for player, Red for hostile, Green for civilian
     * @param uid - Unique ID
     */
    WorldObject(Faction faction, uint32_t uid) : mFaction(faction), mId(uid) {}
    ~WorldObject() override = default;

    Vector getVelVector() const { return mV; }
    Vector getAccVector() const { return mA; }
    Vector getPosVector() const { return mP; }
    qreal getAtan2() const { return mAtan2; }
    QGraphicsItem* getTacticalGraphicsItem() { return mTacticalGraphicsItem; }
    uint32_t getId() const { return mId; }

protected:
    Faction mFaction;
    uint32_t mId;
    qreal mAtan2 = 0; // Bearing (rads)
    Vector mV = Vector(0, 0); // Velocity vector
    Vector mA = Vector(0, 0); // Acceleration vector
    Vector mP = Vector(0, 0); // Position vector
    QGraphicsItem* mTacticalGraphicsItem = nullptr;
};