#include <QtWidgets>

#pragma once

class GridBox : public QGraphicsItem
{
public:
    GridBox(int x, int y) : mX(x), mY(y) { setAcceptHoverEvents(true); }

    constexpr static qreal sGridSize = 10;

    QPoint getCoords() const { return { mX, mY }; }

private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override
    {
        mPen.setColor(QColor(0, 255, 0, 150));
        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override
    {
        mPen.setColor(QColor(0, 255, 0, 25));
        QGraphicsItem::hoverLeaveEvent(event);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(mPen);
        painter->drawRect(QRectF((-sGridSize*2.5) + (mX*sGridSize),  (-sGridSize*2.5) + (mY*sGridSize), sGridSize, sGridSize));
    }

    QRectF boundingRect() const override { return { (-sGridSize*2.5) + (mX*sGridSize),  (-sGridSize*2.5) + (mY*sGridSize), sGridSize, sGridSize }; }

    int mX;
    int mY;
    QPen mPen {QColor(0, 255, 0, 25) };
};