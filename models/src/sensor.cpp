#include "include/sensor.h"


void Sensor::update(QPointF parentPosition, Bearing parentAngle)
{
    if (mIsActive)
    {
        if (mScanCW)
        {
            mScanPosition += mScanSpeed;
            qreal delta = mScanPosition + mScanFOV - mTotalFOV;
            if (delta > 0)
            {
                mScanCW = false;
                mScanPosition -= delta;
            }
        }
        else
        {
            mScanPosition -= mScanSpeed;
            qreal delta = mScanPosition - mScanFOV + mTotalFOV;
            if (delta < 0)
            {
                mScanCW = true;
                mScanPosition += delta;
            }
        }
    }
    if (mItem)
        mItem->updateScan(parentPosition, parentAngle + mBoreAngleOffset, mScanPosition);
}

bool Sensor::withinFOV(qreal offBoreAngle) const
{
    if (!mIsActive) return false;

    qreal delta = offBoreAngle - mBoreAngleOffset - mScanPosition;
    if (delta > M_PI) delta = -(2.0*M_PI) + delta;
    if (delta < -M_PI) delta = (2.0*M_PI) + delta;

    return qAbs(delta) <= mScanFOV;
}