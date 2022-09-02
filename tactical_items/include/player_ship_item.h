#include "blur.h"
#include "include/engine.h"
#include "include/component.h"

#include <QGraphicsItem>
#include <QtMath>

#pragma once


class PlayerShipItem : public QGraphicsItem {
public:
    PlayerShipItem(qreal* angle) : mAtan2(angle) {};
    enum { Type = 1 };
    int type() const override { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void update() { prepareGeometryChange(); }

    void addEngine(std::shared_ptr<Engine> engine) { mEngines.push_back(engine); }
    void addComponent(std::shared_ptr<Component> component) { mComponents.push_back(component); }
    void reset();

private:
    qreal* mAtan2;
    QVector<std::shared_ptr<Engine>> mEngines;
    QVector<std::shared_ptr<Component>> mComponents;
};