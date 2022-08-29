#include "../physics/include/vector.h"
#include "../views/include/globals.h"

#include <QGraphicsItem>
#include <QtWidgets>

#pragma once


class SensorFOV : public QGraphicsItem {
public:
    SensorFOV(qreal totalFOV, qreal scanFOV) : mTotalFOV(totalFOV), mScanFOV(scanFOV) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateScan(QPointF pos, qreal boreAngle, qreal scanAngle);
    void updateScan(QPointF pos, qreal boreAngle, bool isActive = false);

private:
    qreal mTotalFOV;
    qreal mScanFOV;
    qreal mBoreAngle = 0;
    qreal mScanAngle = 0;
    bool mIsActive = true;
};