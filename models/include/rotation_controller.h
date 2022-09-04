#include <QtWidgets>


class RotationController
{
public:
    RotationController(qreal& rotateVel);
    ~RotationController() = default;

    enum class OneDeg
    {
        Left,
        Right,
        Stop
    };

    void commandNewBearing(qreal angleDelta, qreal maxAccPro, qreal maxAccRetro);

    void update();

    OneDeg getDirection();

private:
    qreal mHalfRotateVel = 0;
    qreal& mRotateVel;
    OneDeg mDirection = OneDeg::Stop;
    bool mIsHalfComplete = false;
};