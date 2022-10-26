#include "include/world_object.h"
#include "include/missile_item.h"
#include "include/radar_sensor.h"


class Missile : public WorldObject
{
public:
    Missile(Faction faction, Vector initialPos, Vector initialVel, qreal atan2, uint32_t uid) : WorldObject(faction, uid)
    {
        mP = initialPos;
        mV = initialVel;
        mMaxRightRotateAcc = 0.001;
        mMaxLeftRotateAcc = 0.001;
        mAtan2 = atan2;
        mTacticalGraphicsItem = new MissileItem(mP, mAtan2);
        mSensors << std::make_shared<RadarSensor>(this,
                                                  0,
                                                  1.5, 1.5,
                                                  3);
    }
    ~Missile() override = default;

    void updatePosition(QPointF offset) override
    {
        switch (mRotationController.getDirection())
        {
            case RotationController::OneDeg::Left:
                mRotA = -mMaxLeftRotateAcc;
                break;
            case RotationController::OneDeg::Right:
                mRotA = mMaxRightRotateAcc;
                break;
            default:;
        }

        mRotV += mRotA * deltaT;
        mAtan2 += mRotV * deltaT;

        mP += mV * deltaT;
        mP += Vector(offset.x(), offset.y());
        mTacticalGraphicsItem->setPos({mP.x(), mP.y()});
    }
};