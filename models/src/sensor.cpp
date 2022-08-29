#include "include/sensor.h"

QVector<Sensor::Track> Sensor::getTracks(const QVector<WorldObject*>& worldObjects)
{
    QVector<Sensor::Track> tracks;
    std::for_each(worldObjects.cbegin(), worldObjects.cend(),
                  [this, &tracks](auto obj)
                  {
                        auto track = computeTrack(obj);
                        if (track.has_value()) tracks << track.value();
                  });
    return tracks;
}

std::optional<Sensor::Track> Sensor::computeTrack(WorldObject* obj)
{
    // TODO: Implement signatures and detection algorithms

    return {};
}

void Sensor::update()
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
        mItem->updateScan(mParent->getPoint(), mBoreAngleOffset - mParent->getAtan2(), mScanPosition);
}