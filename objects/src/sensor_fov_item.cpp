#include "include/sensor_fov_item.h"


void SensorFOV::updateScan(QPointF pos, qreal boreAngle, qreal scanAngle)
{
    mBoreAngle = boreAngle;
    mScanAngle = scanAngle;
    mIsActive = true;
    setPos(pos * gScaleFactor);
    prepareGeometryChange();
}

void SensorFOV::updateScan(QPointF pos, qreal boreAngle, bool isActive)
{
    mBoreAngle = boreAngle;
    mIsActive = isActive;
    setPos(pos * gScaleFactor);
    prepareGeometryChange();
}

void SensorFOV::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int fovAngle16Start = int((mBoreAngle + (M_PI * 0.5) + mTotalFOV) * 5760 / (2 * M_PI));
    int fovAngle16Width = -int((mTotalFOV*2) * 5760 / (2 * M_PI));

    QPen pen;
    pen.setColor(QColor(0, 255, 0, 50));
    pen.setWidthF(2.0);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPie(QRectF(-600, -600, 1200, 1200), fovAngle16Start, fovAngle16Width);

    if (!mIsActive)
        return;
    int scanAngle16Start = int((mBoreAngle + mScanAngle + (M_PI * 0.5) + mScanFOV) * 5760 / (2 * M_PI));
    int scanAngle16Width = -int((mScanFOV*2) * 5760 / (2 * M_PI));

    QBrush brush;
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(QColor(0, 255, 0, 25));
    painter->setBrush(brush);
    painter->drawPie(QRectF(-600, -600, 1200, 1200), scanAngle16Start, scanAngle16Width);
}

QRectF SensorFOV::boundingRect() const
{
    return {-600, -600, 1200, 1200};
}