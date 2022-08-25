#include "include/player_ship_item.h"

void PlayerShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    QBrush fillBrush;

    fillBrush.setStyle(Qt::BrushStyle::SolidPattern);
    fillBrush.setColor(QColor(0, 255, 0));

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->rotate(*mAtan2*360.0/(M_PI*2.0));

    for (auto const &e : mEngines)
    {
        pen.setColor(QColor(0, int(255.0*e->getNormTemperature()), 0));
        painter->setPen(pen);

        fillBrush.setColor(QColor(0, int(255.0*e->getOpacity()), 0));
        painter->setBrush(fillBrush);

        painter->drawPolygon(e->getPoly());
    }
    painter->setBrush({});
    for (auto const &c : mComponents)
    {
        pen.setColor(QColor(0, int(255.0*c->getNormTemperature()), 0));
        painter->setPen(pen);
        painter->setBrush({});
        painter->drawPolygon(c->getPoly());
        if (!c->getTexturePoly().isEmpty())
        {
            painter->setPen({0, 0, 0, 0});
            painter->setBrush(QColor(0, int(255.0*c->getNormTemperature()), 0));
            painter->drawPolygon(c->getTexturePoly());
        }
    }
    //painter->rotate(-mAtan2*360.0/(M_PI*2.0));
    /*
    QPolygonF poly = QPolygonF() << QPointF(qCos(mAtan2)*50.0, qSin(mAtan2)*50.0)
                                 << QPointF(qCos(mAtan2 + M_PI - 0.5)*50.0, qSin(mAtan2 + M_PI - 0.5)*50.0)
                                 << QPointF(qCos(mAtan2 + M_PI)*25.0, qSin(mAtan2 + M_PI)*25.0)
                                 << QPointF(qCos(mAtan2 + M_PI + 0.5)*50.0, qSin(mAtan2 + M_PI + 0.5)*50.0);
    painter->drawPolygon(poly);
    */
    painter->rotate(-*mAtan2*360.0/(M_PI*2.0));
    //painter->drawLine(0, 0, mV.x()*10, -mV.y()*10);
}

QRectF PlayerShipItem::boundingRect() const
{
    // Making the bounding rect too big helps with rendering fast moving PlayerShips
    return QRectF(-50, -50, 100, 100);
}

void PlayerShipItem::reset()
{
    mEngines.clear();
    mComponents.clear();
}
