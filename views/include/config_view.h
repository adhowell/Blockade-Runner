#include "include/component.h"
#include "include/engine.h"
#include "include/menu_component.h"
#include "include/text_box.h"

#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once


class PlayerShip;

/**
 * Graphics view for adding and removing components from the
 * player ship.
 */
class ConfigView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ConfigView(QGraphicsScene* scene);

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

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

/**
 * Graphics scene for displaying the ship component menu,
 * the current ship components, and ship specs.
 */
class ConfigScene : public QGraphicsScene
{
Q_OBJECT
public:
    explicit ConfigScene(QWidget *parent = nullptr);

    ConfigView* getView() const;

public Q_SLOTS:
    void handleClose();
    void updateStats(QString mass, QString linearAcc, QString leftAcc, QString rightAcc, QString hasSensors);
    void drawConfigComponent(std::shared_ptr<Component> component);
    void drawConfigEngine(std::shared_ptr<Engine> engine);
    void drawCentreOfMass(qreal x, qreal y);
    void drawCentreOfRotation(qreal x, qreal y);
    void deleteAllComponents();

Q_SIGNALS:
    void close();

private:
    ConfigView* mView;
    QVector<QGraphicsItem*> mTempItems;
    TextBox* mTextMass;
    TextBox* mTextLinearAcc;
    TextBox* mTextLeftAcc;
    TextBox* mTextRightAcc;
    TextBox* mTextSensors;
};