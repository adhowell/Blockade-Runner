#include "include/sensor.h"


void Sensor::update(QPointF parentPosition, qreal parentAngle)
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
    return offBoreAngle > mBoreAngleOffset + mScanPosition - mScanFOV && offBoreAngle < mBoreAngleOffset + mScanPosition + mScanFOV;
}