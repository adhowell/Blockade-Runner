#include "include/engine.h"

Engine::Engine(Component* component, TwoDeg direction, Vector centreOfMassOffset,
               qreal mass, qreal inertia, qreal thrust, qreal incr,
               Profile profile, Size size)
{
    mComponent = component;
    mTemperature = component->getTemperature();
    mDirection = direction;
    mThrust = thrust;
    mIncr = incr;
    mThrustRatioFunction = profile;
    mSize = size;

    // Direction of thrust vector
    switch (direction) {
        case TwoDeg::Up:
            mForwardAcc = (mThrust / mass);
            mRotateAcc = qSin(centreOfMassOffset.separationAngle(Vector(0, -1)))
                         * mThrust * centreOfMassOffset.getSize() / inertia;
            break;
        case TwoDeg::Down:
            mForwardAcc = (-mThrust / mass);
            mRotateAcc = qSin(centreOfMassOffset.separationAngle(Vector(0, 1)))
                        * mThrust * centreOfMassOffset.getSize() / inertia;
            break;
        case TwoDeg::Left:
            mLateralAcc = (-mThrust / mass);
            mRotateAcc = qSin(centreOfMassOffset.separationAngle(Vector(-1, 0)))
                         * mThrust * centreOfMassOffset.getSize() / inertia;
            break;
        case TwoDeg::Right:
            mLateralAcc = (mThrust / mass);
            mRotateAcc = qSin(centreOfMassOffset.separationAngle(Vector(1, 0)))
                         * mThrust * centreOfMassOffset.getSize() / inertia;
            break;
    }
    // Rounding down due to fp error
    if (qAbs(mForwardAcc) < 0.0001) mForwardAcc = 0;
    if (qAbs(mLateralAcc) < 0.0001) mLateralAcc = 0;
    if (qAbs(mRotateAcc) < 0.00000001) mRotateAcc = 0;
}

bool Engine::isForwardAcc() const
{
    return (mForwardAcc > 0);
}

bool Engine::isBackwardAcc() const
{
    return (mForwardAcc < 0);
}

bool Engine::isLateralLeftAcc() const
{
    return (mLateralAcc < 0);
}

bool Engine::isLateralRightAcc() const
{
    return (mLateralAcc > 0);
}

bool Engine::isRotateLeftAcc() const
{
    return (mRotateAcc < 0);
}

bool Engine::isRotateRightAcc() const
{
    return (mRotateAcc > 0);
}

qreal Engine::getLongitudinalAcc() const
{
    return mForwardAcc * mThrustRatio;
}

qreal Engine::getLateralAcc() const
{
    return mLateralAcc * mThrustRatio;
}

qreal Engine::getRotationalAcc() const
{
    return mRotateAcc * mThrustRatio;
}

void Engine::incrementAccProfile()
{
    mTemperature += mThrust*0.1;
    mComponent->applyTemperatureDelta(mThrust*0.05);

    if (mThrustRatioStep < 1) {
        mThrustRatioStep = qMin(mThrustRatioStep + mIncr, 1.0);
        updateThrustRatio();
    }
    mEnabled = true;
}

void Engine::decrementAccProfile()
{
    mTemperature -= mThrust*0.1;
    mComponent->applyTemperatureDelta(-mThrust*0.05);
    if (mTemperature < 0) mTemperature = 0;

    if (mThrustRatioStep > 0) {
        mThrustRatioStep = qMax(mThrustRatioStep - mIncr, 0.0);
        updateThrustRatio();
    }
    if (mThrustRatioStep == 0 && mEnabled)
    {
        mEnabled = false;
        Q_EMIT transmitStatus("<INFO> - THRUSTER SHUTDOWN SUCCESS");
    }
}

void Engine::updateThrustRatio()
{
    switch (mThrustRatioFunction) {
        case Profile::EXP:
            mThrustRatio = qExp(mThrustRatioStep - 1.0);
            break;
        case Profile::LIN:
            mThrustRatio = mThrustRatioStep;
            break;
    }
}

void Engine::createPoly(QPointF centre)
{
    int height;
    int minWidth;
    int maxWidth;
    switch (mSize)
    {
        case BIG:
            height = 10;
            minWidth = 3;
            maxWidth = 5;
            break;
        case SMALL:
            height = 3;
            minWidth = 2;
            maxWidth = 4;
    }

    switch (mDirection)
    {
        case TwoDeg::Up:
            mPoly << centre + QPointF(-minWidth, 0) << centre + QPointF(-maxWidth, height)
                  << centre + QPointF(maxWidth, height) << centre + QPointF(minWidth, 0);
            break;
        case TwoDeg::Right:
            mPoly << centre + QPointF(0, -minWidth) << centre + QPointF(-height, -maxWidth)
                  << centre + QPointF(-height, maxWidth) << centre + QPointF(0, minWidth);
            break;
        case TwoDeg::Down:
            mPoly << centre + QPointF(-minWidth, 0) << centre + QPointF(-maxWidth, -height)
                  << centre + QPointF(maxWidth, -height) << centre + QPointF(minWidth, 0);
            break;
        case TwoDeg::Left:
            mPoly << centre + QPointF(0, -minWidth) << centre + QPointF(height, -maxWidth)
                  << centre + QPointF(height, maxWidth) << centre + QPointF(0, minWidth);
            break;
    }
}