#include "include/world_object.h"
#include "include/asteroid.h"


class Missile : public WorldObject
{
public:
    Missile(Faction faction, Vector initialPos, uint32_t uid) : WorldObject(faction, uid)
    {
        mP = initialPos;
        mTacticalGraphicsItem = new Asteroid(QColor(255, 0, 0), initialPos.x(), initialPos.y(), {0, 0}, 1, 500);
    }
    ~Missile() override = default;

    void updatePosition(qreal mDeltaT, QPointF offset) override
    {
        mP += Vector(offset.x(), offset.y());
        mTacticalGraphicsItem->setPos(offset);
    }
};