#include "include/world_object.h"
#include "include/missile_item.h"
#include "include/radar_sensor.h"


class Missile : public WorldObject
{
public:
    Missile(Faction faction, Vector initialPos, Vector initialVel, qreal atan2, uint32_t uid);
    ~Missile() override = default;

    void updatePosition(QPointF offset) override;

private:
    qreal mThrust {0.001f};
};