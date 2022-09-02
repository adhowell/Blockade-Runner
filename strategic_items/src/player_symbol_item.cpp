#include "include/player_symbol_item.h"

PlayerSymbolItem::PlayerSymbolItem(QPointF pos) : mVelocity(0, 0)
{
    mAtan2 = 0;
}

void PlayerSymbolItem::applyUpdate(qreal angle, Vector velocity)
{
    mAtan2 = angle - 0.5*M_PI;
    mVelocity = velocity;
    mPoly.clear();
    mPoly << QPointF(qCos(mAtan2)*mHalfLength, qSin(mAtan2)*mHalfLength)
          << QPointF(qCos(mAtan2 + M_PI - 0.5)*mHalfLength, qSin(mAtan2 + M_PI - 0.5)*mHalfLength)
          << QPointF(qCos(mAtan2 + M_PI + 0.5)*mHalfLength, qSin(mAtan2 + M_PI + 0.5)*mHalfLength);
    prepareGeometryChange();
}

void PlayerSymbolItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QColor(0, 255, 0));
    painter->setBrush(QColor(0, 255, 0));

    painter->drawPolygon(mPoly);
}

QRectF PlayerSymbolItem::boundingRect() const
{
    return {-mHalfLength, -mHalfLength, mHalfLength * 2, mHalfLength * 2};
}
