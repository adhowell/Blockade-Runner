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
    int sceneInitX = (int)mMinX + (int)(mOrigin.x()) % mSpacing;
    int sceneInitY = (int)mMinY + (int)(mOrigin.y()) % mSpacing;
    int sceneX = sceneInitX;
    int sceneY = sceneInitY;
    qreal remX = mOrigin.x() - xOffset;
    qreal remY = mOrigin.y() - yOffset;

    QPen pen;
    pen.setColor(QColor(0, 255, 0, 25));
    pen.setWidthF(2.0);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    for (int x = xOffset; x < xOffset + mWidth; x += mSpacing)
    {
        painter->drawLine(QLineF(sceneX+remX, mMinY, sceneX+remX, mMinY+mHeight));
        sceneX += mSpacing;
    }
    for (int y = yOffset; y < yOffset + mHeight; y += mSpacing)
    {
        painter->drawLine(QLineF(mMinX, sceneY+remY, mMinX+mWidth, sceneY+remY));
        sceneY += mSpacing;
    }
}

QRectF GridLines::boundingRect() const
{
    return {mMinX, mMinY, mWidth, mHeight};
}