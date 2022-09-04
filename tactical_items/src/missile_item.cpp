#include "include/missile_item.h"


MissileItem::MissileItem(Vector& pos, qreal& atan2) : mPos(pos), mAtan2(atan2)
{
}


void MissileItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    pen.setColor(QColor(0, 255, 0));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);

    painter->rotate(mAtan2*180.0/M_PI);
    painter->drawRect(QRectF(-mWidth, -mLength, mWidth*2.0, mLength*2.0));
    painter->drawRect(QRectF(-mWidth,  mLength, mWidth*2.0, mWidth*2.0));
    painter->rotate(-mAtan2*180.0/M_PI);
}

QRectF MissileItem::boundingRect() const
{
    // Making the bounding rect too big helps with rendering fast moving missiles
    return QRectF(-mLength*2, -mLength*2, mLength*4.0, mLength*4.0);
}
