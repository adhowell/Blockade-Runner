#include "include/sensor_fov_item.h"

#include <QtWidgets>

#pragma once


class Sensor
{
public:
    enum class Radiation
    {
        None,
        Infrared,
        Radio,
        UV,
    };

    Sensor(Radiation radTx, Radiation radRx, qreal txPower, qreal rxPower, qreal boreAngleOffset, qreal leftOffset, qreal rightOffset, qreal scanFOV, qreal scanSpeed)
            : mRadTx(radTx), mRadRx(radRx), mTxPower(txPower), mRxPower(rxPower), mLeftFOVLimit(leftOffset), mRightFOVLimit(rightOffset), mBoreAngleOffset(boreAngleOffset), mScanSpeed(scanSpeed)
            {
                mSweepEnabled = 0.4*(mLeftFOVLimit + mRightFOVLimit) > scanFOV;
                if (mSweepEnabled) {
                    mScanFOV = scanFOV;
                } else {
                    mScanFOV = 0.5*(leftOffset + rightOffset);
                }
                mScanPosition = 0.5*(mRightFOVLimit - mLeftFOVLimit);
                mItem = new SensorFOV(mLeftFOVLimit, mRightFOVLimit, mScanFOV);
            }
    ~Sensor() = default;

    QGraphicsItem* getItem() { return mItem; }

    /**
     * Update the scan angle and the associated graphics item (if any).
     */
    void update(QPointF parentPosition, Bearing parentAngle);

    /**
     * Determines if the given angle is inside the sensor FOV.
     *
     * @param offBoreAngle - Angle relative to the longitudinal axis of the parent.
     * @return True if the given angle is within the sensor FOV.
     */
    bool withinFOV(qreal offBoreAngle) const;

protected:
    Radiation mRadTx; // The radiation the sensor emits
    Radiation mRadRx; // The radiation the sensor receives

    qreal mBoreAngleOffset; // The offset angle from the parent bore

    qreal mTxPower; // Total transmit power
    qreal mRxPower; // Receiver noise floor

    qreal mLeftFOVLimit;
    qreal mRightFOVLimit;
    qreal mScanFOV;
    qreal mScanSpeed;
    qreal mScanPosition = 0; // The bearing (rads) the scan cone is pointing
    bool mScanCW = true; // Scan direction
    bool mIsActive = true;
    bool mSweepEnabled = true;

    SensorFOV* mItem;
};