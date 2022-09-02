#include "include/acceleration_marker.h"


void AccelerationMarker::updateOffset(Vector vel, Vector acc)
{
    mOrigin = (vel * mSeconds) + (acc * mSeconds * mSeconds * 0.5);
    setPos(mOrigin.getSize() * qSin(mOrigin.getAtan2()), -mOrigin.getSize() * qCos(mOrigin.getAtan2()));
    prepareGeometryChange();
}

void AccelerationMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(QColor(0, 255, 0));
    pen.setWidthF(1.0);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(-mSize, -mSize, mSize*2, mSize*2);
}

QRectF AccelerationMarker::boundingRect() const
{
    return {-20, -20, 40, 40};
}