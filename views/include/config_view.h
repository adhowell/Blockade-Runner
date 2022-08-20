#include "../physics/include/component.h"
#include "../physics/include/engine.h"
#include "../objects/include/menu_component.h"

#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once

class PlayerShip;

class ConfigView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ConfigView(QGraphicsScene* scene);

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

Q_SIGNALS:
    void addShipPart(Component::ComponentType, QPoint, TwoDeg);
    void removeShipPart(QPoint);
    void handleClose();

private:
    void attemptFocusTile(QPoint pos);
    void attemptAddPart(QPoint pos);
    void attemptRemovePart(QPoint pos);

    void updateGridBoxes();

    Component::ComponentType mFocusComponent = Component::ComponentType::Reactor;
    TwoDeg mDirection = TwoDeg::Up;
    bool mItemRequiresDirection = false;

};

class ConfigScene : public QGraphicsScene
{
Q_OBJECT
public:
    explicit ConfigScene(QWidget *parent = nullptr);

    void updateItems(QPointF offset);
    ConfigView* getView() const;

public Q_SLOTS:
    void handleClose();
    void drawConfigComponent(std::shared_ptr<Component> component);
    void drawConfigEngine(std::shared_ptr<Engine> engine);
    void deleteAllComponents();

Q_SIGNALS:
    void close();

private:
    ConfigView* mView;
    QVector<QGraphicsItem*> mComponents;
};