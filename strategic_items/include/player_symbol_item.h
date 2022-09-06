#include "include/vector.h"
#include "include/bearing.h"

#include <QGraphicsItem>
#include <QtWidgets>


class PlayerSymbolItem : public QGraphicsItem {
public:
    explicit PlayerSymbolItem(QPointF pos);
    enum { Type = 5 };
    int type() const override { return Type; }

    /**
     * Move the player ship symbol by the given offset.
     *
     * @param angle - The bearing (in radians) of the player ship.
     */
    void applyUpdate(qreal angle);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void update() {}

private:
    qreal mHalfLength = 10;
    qreal mMaxVelocityLength = 30;

    QPolygonF mPoly;
    Bearing mAtan2;
    Vector mVelocity;
};