#include "include/rotation_controller.h"

RotationController::RotationController(Bearing& bearing, qreal& rotateVel, qreal& maxCWRotateAcc, qreal& maxCCWRotateAcc)
: mBearing(bearing), mRotateVel(rotateVel), mMaxCWRotateAcc(maxCWRotateAcc), mMaxCCWRotateAcc(maxCCWRotateAcc)
{
    mTargetBearing = mBearing;
}

void RotationController::update()
{
    qreal delta = mBearing.getDelta(mTargetBearing);
    OneDeg burnDirection = delta > 0 ? OneDeg::Right : OneDeg::Left;
    qreal acc = (mRotateVel * mRotateVel) / (2.0f * delta);
    if (acc > 0) {
        mDirection = mMaxCWRotateAcc <= acc ? OneDeg::Left : burnDirection;
    } else {
        mDirection =  mMaxCCWRotateAcc <= -acc ? OneDeg::Right : burnDirection;
    }
    if (qAbs(mRotateVel) < 0.1 && qAbs(delta) < 0.01) {
        mState = State::MaintainHeading;
    }
}

void RotationController::updatePWM()
{
    if (mPWMCycle > 0) {
        mDirection = OneDeg::Wait;
        mPWMCycle--;
    } else {
        qreal delta = mBearing.getDelta(mTargetBearing);

        // Large corrections (e.g. due to asymmetric engines) are better suited to align-mode
        if (qAbs(delta) > 0.05) {
            mState = State::AlignToTarget;
            return;
        }

        // On sim start
        if (delta == 0) {
            mDirection = OneDeg::Wait;
            return;
        }

        OneDeg burnDirection = delta > 0 ? OneDeg::Right : OneDeg::Left;
        qreal acc = (mRotateVel * mRotateVel) / (2.0f * delta);
        if (acc > 0) {
            mPWMCycle = (uint32_t) (mMaxCWRotateAcc / acc);
        } else {
            mPWMCycle = (uint32_t) (mMaxCCWRotateAcc / -acc);
        }
        mPWMCycle >>= 2;
        mDirection = burnDirection;

        if (mPWMCycle == 0 && qAbs(mRotateVel < 0.0001) && qAbs(delta) < 0.01) {
            mPWMCycle = 1;
            mDirection = OneDeg::Wait;
        }
    }

}

RotationController::OneDeg RotationController::getDirection()
{
    switch (mState)
    {
        case State::AlignToTarget:
            update();
            break;
        case State::MaintainHeading:
            updatePWM();
            break;
    }
    return mDirection;
}