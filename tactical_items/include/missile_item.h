#include "include/vector.h"

#include <QGraphicsItem>
#include <QtMath>


/**
 * Physics-enabled Asteroid graphics item
 */
class MissileItem : public QGraphicsItem
{
public:
    MissileItem(Vector& pos, Bearing& atan2);
    enum { Type = 7 };
    int type() const override { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    static qreal sMinMass;
    static qreal sMaxMass;
    static qreal sMinRadius;
    static qreal sMaxRadius;

private:
    Vector& mPos;  // Centre point
    Bearing& mAtan2;

    qreal mLength = 50;
    qreal mWidth = 10;
};