#include "include/velocity_marker.h"

void VelocityMarker::updateOffset(Vector vel, Vector acc)
{
    mOrigin = (vel * mSeconds) + (acc * mSeconds * mSeconds * 0.5);
    setPos(mOrigin.getSize() * qSin(mOrigin.getAtan2()), -mOrigin.getSize() * qCos(mOrigin.getAtan2()));
    prepareGeometryChange();
}

void VelocityMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(QColor(0, 255, 0));
    pen.setWidthF(2.0);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(-mSize, -mSize, mSize*2, mSize*2);
}

QRectF VelocityMarker::boundingRect() const
{
    return {-20, -20, 40, 40};
}