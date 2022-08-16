#include <QtWidgets>

#pragma once

class GridBox : public QGraphicsItem
{
public:
    GridBox(int x, int y) : mX(x), mY(y)
    {
        setAcceptHoverEvents(true);
        QVector<QPointF> p;
        p << QPointF((-sGridSize*2) + (mX*sGridSize) - 3,  (-sGridSize*2) + (mY*sGridSize));
        p << QPointF((-sGridSize*2) + (mX*sGridSize),  (-sGridSize*2) + (mY*sGridSize) - 3);
        p << QPointF((-sGridSize*2) + (mX*sGridSize) + 3,  (-sGridSize*2) + (mY*sGridSize));
        p << QPointF((-sGridSize*2) + (mX*sGridSize),  (-sGridSize*2) + (mY*sGridSize) + 3);
        mTriangles[TwoDeg::Up] << p[0] << p[1] << p[2];
        mTriangles[TwoDeg::Right] << p[1] << p[2] << p[3];
        mTriangles[TwoDeg::Down] << p[2] << p[3] << p[0];
        mTriangles[TwoDeg::Left] << p[3] << p[0] << p[1];
    }

    constexpr static qreal sGridSize = 10;

    QPoint getCoords() const { return { mX, mY }; }

    void setDirection(TwoDeg d) { mDirection = d; }
    void setDrawDirection(bool b) { mDrawDirection = b; }

private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override
    {
        mPen.setColor(QColor(0, 255, 0, 150));
        mIsFocus = true;
        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override
    {
        mPen.setColor(QColor(0, 255, 0, 25));
        mIsFocus = false;
        QGraphicsItem::hoverLeaveEvent(event);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(mPen);
        painter->drawRect(QRectF((-sGridSize*2.5) + (mX*sGridSize),  (-sGridSize*2.5) + (mY*sGridSize), sGridSize, sGridSize));
        if (mIsFocus && mDrawDirection)
            painter->drawPolygon(mTriangles[mDirection]);
    }

    QRectF boundingRect() const override { return { (-sGridSize*2.5) + (mX*sGridSize),  (-sGridSize*2.5) + (mY*sGridSize), sGridSize, sGridSize }; }

    int mX;
    int mY;
    bool mIsFocus = false;
    bool mDrawDirection = false;
    TwoDeg mDirection = TwoDeg::Up;
    QPen mPen {QColor(0, 255, 0, 25) };
    QMap<TwoDeg, QPolygonF> mTriangles;
};