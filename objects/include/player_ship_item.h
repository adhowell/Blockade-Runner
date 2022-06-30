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

    void addEngine(Engine* engine) { mEngines.push_back(engine); }
    void addComponent(Component* component) { mComponents.push_back(component); }

private:
    qreal* mAtan2;
    QVector<Engine*> mEngines;
    QVector<Component*> mComponents;
};