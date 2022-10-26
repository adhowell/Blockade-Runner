#include "include/missile.h"


Missile::Missile(Faction faction, Vector initialPos, Vector initialVel, qreal atan2, uint32_t uid)
: WorldObject(faction, uid)
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

void Missile::updatePosition(QPointF offset)
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
    mA += Vector(mAtan2) * mThrust;
    mV += mA * deltaT;
    mP += mV * deltaT;
    mP += Vector(offset.x(), offset.y());
    mTacticalGraphicsItem->setPos({mP.x(), mP.y()});
}