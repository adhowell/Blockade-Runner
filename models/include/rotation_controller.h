#include "include/bearing.h"

#include <QtWidgets>


class RotationController
{
public:
    RotationController(Bearing& bearing, qreal& rotateVel, qreal& maxCWRotateAcc, qreal& maxCCWRotateAcc);
    ~RotationController() = default;

    enum class State
    {
        AlignToTarget, // Suicide burn to stop exactly at the target bearing
        MaintainHeading // Try to keep aligned with the target bearing
    };

    enum class OneDeg
    {
        Left,
        Right,
        Stop,
        Wait
    };

    void commandNewBearing(qreal angleDelta)
    {
        mTargetBearing = mBearing + angleDelta;
        mState = State::AlignToTarget;
    }
    void commandNewBearing(Bearing targetBearing) { mTargetBearing = targetBearing; mState = State::AlignToTarget; }

    void update();
    void updatePWM();

    OneDeg getDirection();

private:
    Bearing& mBearing;
    Bearing mTargetBearing {0};
    qreal& mRotateVel;
    qreal& mMaxCWRotateAcc;
    qreal& mMaxCCWRotateAcc;
    OneDeg mDirection = OneDeg::Stop;
    State mState = State::MaintainHeading;
    uint32_t mPWMCycle {0};
};