#include "include/vector.h"
#include "include/faction.h"

#include <QGraphicsItem>
#include <QtMath>


/**
 * Physics-enabled Asteroid graphics item
 */
class StrategicSymbol : public QGraphicsItem
{
public:
    StrategicSymbol()
    {
        mPoly = QPolygonF() << QPointF(-mSize, 0)
                            << QPointF(0, -mSize)
                            << QPointF(mSize, 0)
                            << QPointF(0, mSize);
    }
    ~StrategicSymbol() override = default;
    enum { Type = 6 };
    int type() const override { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateTrack(qreal x, qreal y, Vector velocity, Faction perceivedFaction, bool isCurrent);
    void updateOffset(QPointF offset);

private:
    QPolygonF mPoly;
    QLineF mVelLine {0, 0, 0, 0};
    Faction mPerceivedFaction {Faction::Unknown};
    QColor mColour;
    const qreal mSize {15};
    int16_t mLifetime {0};
    int16_t mMaxLifetime {1000};
    int16_t mAnimationLifetime {0};
    int16_t mAnimationMaxLifetime {225};
    bool mDrawBox {false};
    bool mIsCurrent {false};
};