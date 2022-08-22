#include "include/starfield.h"

void Starfield::updateOffset(QPointF offset)
{
    mOrigin += offset;
    mLastOffset = offset;
    prepareGeometryChange();
}

void Starfield::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int xOffset = (int)mOrigin.x();
    int yOffset = (int)mOrigin.y();
    int sceneInitX = -1000 + (int)(mOrigin.x()) % 10;
    int sceneInitY = -700 + (int)(mOrigin.y()) % 10;
    int sceneX = sceneInitX;
    int sceneY = sceneInitY;
    qreal remX = mOrigin.x() - (int)mOrigin.x();
    qreal remY = mOrigin.y() - (int)mOrigin.y();

    QPen pen;
    pen.setColor(QColor(0, 100, 0));
    painter->setPen(pen);

    // TODO: Improve the procedural generation seeding logic
    for (int x = xOffset; x < xOffset + 2000; x += 10)
    {
        for (int y = yOffset; y < yOffset + 1400; y += 10)
        {
            uint32_t xHash = uint32_t(qAbs(sceneX - xOffset)) << 16;
            uint32_t yHash = uint32_t(qAbs(sceneY - yOffset));
            if (sceneX - xOffset < 0) xHash |= 0x80000000;
            if (sceneY - yOffset < 0) yHash |= 0x8000;

            uint32_t prng = lcg_parkmiller(xHash | yHash) & 0x2AAB;
            if (prng == 0x2AAA)
            {
                painter->drawLine(QLineF(sceneX+remX, sceneY+remY,
                                         sceneX+remX+mLastOffset.x(), sceneY+remY+mLastOffset.y()));
            }
            sceneY += 10;
        }
        sceneY = sceneInitY;
        sceneX += 10;
    }
}

uint32_t Starfield::lcg_parkmiller(uint32_t state)
{
    uint64_t product = (uint64_t)state * 48271;
    uint32_t x = (product & 0x7fffffff) + (product >> 31);

    x = (x & 0x7fffffff) + (x >> 31);
    return x;
}

QRectF Starfield::boundingRect() const
{
    return {-1000, -700, 2000, 1400};
}