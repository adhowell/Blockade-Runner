#include "include/phosphor_ghost.h"


PhosphorGhost::PhosphorGhost(QPolygonF poly, int lifetime) : mPoly(poly), mLifetime(lifetime)
{
    mBlurEffect = new Blur(this);
    setGraphicsEffect(mBlurEffect);
}

void PhosphorGhost::posUpdate(QPointF posDelta)
{
    mP += posDelta;
}

void PhosphorGhost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QPen pen;

    pen.setColor(QColor(0, 255, 0));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);

    painter->drawPolygon(mPoly);

}

QRectF PhosphorGhost::boundingRect() const
{
    return QRectF(-100, -100, 200, 200);
}

void PhosphorGhost::update()
{
    setPos(mP);
    mLifetime--;
    mBlurEffect->incrRadius();
    mBlurEffect->adjustForItem();
}