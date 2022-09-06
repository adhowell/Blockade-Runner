#include "include/vector.h"
#include "include/globals.h"

#include <QGraphicsItem>
#include <QtWidgets>

#pragma once


class SensorFOV : public QGraphicsItem {
public:
    SensorFOV(qreal leftFOV, qreal rightFOV, qreal scanFOV) : mLeftFOV(leftFOV), mRightFOV(rightFOV), mTotalFOV(leftFOV + rightFOV), mScanFOV(scanFOV) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateScan(QPointF pos, qreal boreAngle, qreal scanAngle);
    void updateScan(QPointF pos, qreal boreAngle, bool isActive = false);

private:
    qreal mLeftFOV;
    qreal mRightFOV;
    qreal mTotalFOV;
    qreal mScanFOV;
    qreal mBoreAngle = 0;
    qreal mScanAngle = 0;
    bool mIsActive = true;
};