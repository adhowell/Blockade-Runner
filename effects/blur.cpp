#include "blur.h"


Blur::Blur(QGraphicsItem *item) : QGraphicsBlurEffect()
{
    setBlurHints(QGraphicsBlurEffect::AnimationHint);
}

void Blur::adjustForItem()
{
    setBlurRadius(mRadius);
}

void Blur::draw(QPainter *painter)
{
    adjustForItem();
    QGraphicsBlurEffect::draw(painter);
}