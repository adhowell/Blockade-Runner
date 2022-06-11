#include <QGraphicsBlurEffect>
#include <QGraphicsItem>

#pragma once

class Blur : public QGraphicsBlurEffect
{
public:
    Blur(QGraphicsItem* item);

    void draw(QPainter *painter) override;

    void setRadius(qreal radius) { mRadius = radius; }
    void incrRadius() { mRadius += 5; }
    void adjustForItem();

private:
    qreal mRadius = 1;
};