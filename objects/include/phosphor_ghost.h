#include "blur.h"
#include <QGraphicsItem>
#include <QtWidgets>

class PhosphorGhost : public QGraphicsItem {
public:
    PhosphorGhost(QPolygonF poly, int lifetime);
    enum { Type = 4 };
    int type() const override { return Type; }

    /**
     * Move the ghost by the given offset.
     * @param posDelta offset to apply to current position
     */
    void posUpdate(QPointF posDelta);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void update();

    bool isDone() { return mLifetime <= 1; }

private:
    QPolygonF mPoly;
    QPointF mP = QPointF(); // Position
    int mLifetime;
    Blur* mBlurEffect;
};