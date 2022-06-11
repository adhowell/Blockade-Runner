#ifndef STARFIELD_H
#define STARFIELD_H
#include <QGraphicsItem>
#include <QtWidgets>

class Starfield : public QGraphicsItem {
public:
    explicit Starfield(QPointF origin) : mOrigin(origin) {}

    /**
     * Fast PRNG. Nicked from: https://en.wikipedia.org/wiki/Lehmer_random_number_generator
     * @param state Previous or initial (i.e. seed) state
     */
    static uint32_t lcg_parkmiller(uint32_t state);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateOffset(QPointF offset);

private:
    QPointF mOrigin;
    QPointF mLastOffset;
};

#endif //STARFIELD_H
