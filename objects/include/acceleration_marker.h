#include "../physics/include/vector.h"

#include <QGraphicsItem>
#include <QtWidgets>

#pragma once

class AccelerationMarker : public QGraphicsItem {
public:
    explicit AccelerationMarker(int seconds) : mOrigin(0, 0), mSeconds(seconds), mSize(seconds/600) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateOffset(Vector vel, Vector acc);

private:
    Vector mOrigin;
    qreal mSeconds;
    int mSize;
};