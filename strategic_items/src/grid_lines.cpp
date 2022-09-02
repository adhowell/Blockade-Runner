#include "include/grid_lines.h"

void GridLines::updateOffset(QPointF offset)
{
    mOrigin += offset;
    prepareGeometryChange();
}

void GridLines::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int xOffset = (int)mOrigin.x();
    int yOffset = (int)mOrigin.y();
    int sceneInitX = -1000 + (int)(mOrigin.x()) % mSpacing;
    int sceneInitY = -700 + (int)(mOrigin.y()) % mSpacing;
    int sceneX = sceneInitX;
    int sceneY = sceneInitY;
    qreal remX = mOrigin.x() - xOffset;
    qreal remY = mOrigin.y() - yOffset;

    QPen pen;
    pen.setColor(QColor(0, 255, 0, 25));
    pen.setWidthF(2.0);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    for (int x = xOffset; x < xOffset + 2000; x += mSpacing)
    {
        painter->drawLine(QLineF(sceneX+remX, -1000, sceneX+remX, 1000));
        sceneX += mSpacing;
    }
    for (int y = yOffset; y < yOffset + 1400; y += mSpacing)
    {
        painter->drawLine(QLineF(-1000, sceneY+remY, 1000, sceneY+remY));
        sceneY += mSpacing;
    }
}

QRectF GridLines::boundingRect() const
{
    return {-1000, -700, 2000, 1400};
}