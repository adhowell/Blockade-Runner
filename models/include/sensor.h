#include <QtWidgets>
#include "../world_objects/include/world_object.h"
#include "../objects/include/sensor_fov_item.h"

#pragma once


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
        WorldObject::Faction faction;
        uint32_t targetUid;
    };

    enum class Radiation
    {
        None,
        Infrared,
        Radio,
        UV,
    };

    Sensor(WorldObject* parent, Radiation radTx, Radiation radRx, qreal txPower, qreal rxPower, qreal boreAngleOffset, qreal totalFOV, qreal scanFOV, qreal scanSpeed)
            : mParent(parent), mRadTx(radTx), mRadRx(radRx), mTxPower(txPower), mRxPower(rxPower), mBoreAngleOffset(boreAngleOffset), mTotalFOV(totalFOV), mScanFOV(scanFOV), mScanSpeed(scanSpeed)
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
    void update();

protected:
    std::optional<Track> computeTrack(WorldObject* obj);

    WorldObject* mParent;

    Radiation mRadTx; // The radiation the sensor emits
    Radiation mRadRx; // The radiation the sensor receives

    qreal mBoreAngleOffset; // The offset angle from the parent bore

    qreal mTxPower; // Total transmit power
    qreal mRxPower; // Receiver noise floor

    qreal mTotalFOV;
    qreal mScanFOV;
    qreal mScanSpeed;
    qreal mScanPosition; // The bearing (rads) the scan cone is pointing
    bool mScanCW = true; // Scan direction
    bool mIsActive = true;

    SensorFOV* mItem;
};