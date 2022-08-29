#include <QtWidgets>
#include "../objects/include/sensor_fov_item.h"

#pragma once


class WorldObject;

class Sensor
{
public:
    /**
     * A track is a container for the information a sensor knows
     * about a particular object in the world.
     */
    struct Track
    {
        qreal bearingOffBore;
        qreal distance;
        qreal receivedPower;
        //WorldObject::Faction faction;
        uint32_t targetUid;
    };

    enum class Radiation
    {
        None,
        Infrared,
        Radio,
        UV,
    };

    Sensor(Radiation radTx, Radiation radRx, qreal txPower, qreal rxPower, qreal boreAngleOffset, qreal totalFOV, qreal scanFOV, qreal scanSpeed)
            : mRadTx(radTx), mRadRx(radRx), mTxPower(txPower), mRxPower(rxPower), mBoreAngleOffset(boreAngleOffset), mTotalFOV(totalFOV), mScanFOV(scanFOV), mScanSpeed(scanSpeed)
            {
                mItem = new SensorFOV(totalFOV, scanFOV);
            }
    ~Sensor() = default;

    QGraphicsItem* getItem() { return mItem; }

    /**
     * Returns all of the valid tracks of the given objects.
     * @param worldObjects
     * @return
     */
    QVector<Track> getTracks(const QVector<WorldObject*>& worldObjects);

    /**
     * Update the scan angle and the associated graphics item (if any).
     */
    void update(QPointF parentPosition, qreal parentAngle);

protected:
    std::optional<Track> computeTrack(WorldObject* obj);

    Radiation mRadTx; // The radiation the sensor emits
    Radiation mRadRx; // The radiation the sensor receives

    qreal mBoreAngleOffset; // The offset angle from the parent bore

    qreal mTxPower; // Total transmit power
    qreal mRxPower; // Receiver noise floor

    qreal mTotalFOV;
    qreal mScanFOV;
    qreal mScanSpeed;
    qreal mScanPosition = 0; // The bearing (rads) the scan cone is pointing
    bool mScanCW = true; // Scan direction
    bool mIsActive = true;

    SensorFOV* mItem;
};