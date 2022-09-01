#include "include/world_object.h"


class Missile : public WorldObject
{
public:
    Missile(WorldObject::Faction faction, Vector initialPos, uint32_t uid) : WorldObject(faction, uid)
    {
        mP = initialPos;
    }
    ~Missile() override = default;

    void updatePosition(qreal mDeltaT, QPointF offset) override
    {
        mP += Vector(offset.x(), offset.y());
    }
};