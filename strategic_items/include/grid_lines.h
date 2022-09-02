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
};
