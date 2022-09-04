#include "include/rotation_controller.h"

RotationController::RotationController(qreal& rotateVel) : mRotateVel(rotateVel)
{

}

void RotationController::commandNewBearing(qreal angleDelta, qreal maxAccPro, qreal maxAccRetro)
{
    mIsHalfComplete = false;
    mHalfRotateVel = qSqrt(((mRotateVel * mRotateVel) + (2.0 * maxAccPro * qAbs(angleDelta))) / (1.0 + (maxAccPro/maxAccRetro)));
    if (angleDelta < 0) {
        mDirection = OneDeg::Left;
        mHalfRotateVel *= -1.0;
    } else {
        mDirection = OneDeg::Right;
    }
}

void RotationController::update()
{
    if (mDirection == OneDeg::Right && mRotateVel >= mHalfRotateVel)
    {
        mIsHalfComplete = true;
        mDirection = OneDeg::Left;
    }
    else if (mDirection == OneDeg::Left && mRotateVel <= mHalfRotateVel)
    {
        mIsHalfComplete = true;
        mDirection = OneDeg::Right;
    }
}

RotationController::OneDeg RotationController::getDirection()
{
    if (!mIsHalfComplete)
        update();
    if (mIsHalfComplete && qAbs(mRotateVel) < 0.0001)
    {
        mDirection = OneDeg::Stop;
        mIsHalfComplete = false;
    }
    return mDirection;
}