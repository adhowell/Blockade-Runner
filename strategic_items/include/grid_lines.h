#include <QGraphicsItem>
#include <QtWidgets>

#pragma once


class GridLines : public QGraphicsItem {
public:
    GridLines(int spacing) : mOrigin(0, 0), mSpacing(spacing) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateOffset(QPointF offset);

private:
    QPointF mOrigin;
    int mSpacing;
    const qreal mMinX {-2000};
    const qreal mMinY {-1500};
    const qreal mWidth {abs(mMinX)*2};
    const qreal mHeight {abs(mMinY)*2};
};
