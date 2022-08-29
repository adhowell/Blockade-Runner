#include "../world_objects/include/world_object.h"
#include "blur.h"
#include "include/engine.h"
#include "include/component.h"
#include "include/player_ship_item.h"

#include <QGraphicsItem>
#include <QtMath>

#pragma once

class PlayerShip : public WorldObject {
    Q_OBJECT
public:
    PlayerShip(WorldObject::Faction faction, uint32_t uid);

    typedef Component::ComponentType CT;

    enum class RotateState {
        Idle,
        BeforeTargetCW,
        AfterTargetCW,
        BeforeTargetCCW,
        AfterTargetCCW,
        Shutdown,
    };

    static QMap<Component, qreal> sComponentMass;

    void addReactor(int x, int y);
    void addHeatSink(int x, int y);
    void addRotateThruster(int x, int y);
    void addCruiseThruster(int x, int y, TwoDeg direction);

    void computeRotationalInertia();

    /**
     * Iterates over every component in the ship and creates the
     * associated engines.
     */
    void createAllEngines();

    /**
     * Add engines to the player ship based on the component position
     * and type.
     *
     * @param c - The component to use for adding engines.
     */
    void createComponentEngines(std::shared_ptr<Component>& c);

    void computeCentreOfRotation();
    void computeProperties();

    /**
     * Re-computes the player ship using only the component map. This
     * discards the current engines and physical properties.
     */
    void reconfigure();

    /**
     * Sends signals to the tactical view and config menu to display the
     * components, engines and misc items.
     */
    void updateVisuals();

    bool isGridLineFree(int x, int y, TwoDeg direction);

    template<class T>
    void computeEngineDirectionForce(int x, int y, TwoDeg direction);

    /**
     * Update the position and orientation of the ship using the given delta-t.
     *
     * @param deltaT - The time step value.
     */
    void update(qreal deltaT);

    void resetMovement();
    void enableForward() { mForwardThrust = true; }
    void enableBackward() { mBackwardThrust = true; }
    void enableLateralLeft() { mLeftThrust = true; }
    void enableLateralRight() { mRightThrust = true; }
    void enableRotateLeft() { mRotateLeftThrust = true; }
    void enableRotateRight() { mRotateRightThrust = true; }

    void rotate(int degrees);

public Q_SLOTS:
    void receiveTextFromComponent(const QString& text);
    void handleAddPart(Component::ComponentType, QPoint, TwoDeg);
    void handleRemovePart(QPoint);

Q_SIGNALS:
    void displayText(QString text);
    void handleAddConfigComponent(std::shared_ptr<Component>);
    void handleAddConfigEngine(std::shared_ptr<Engine>);
    void handleAddCentreOfMass(qreal, qreal);
    void handleAddCentreOfRotation(qreal, qreal);
    void handleRemoveAllConfigItems();

private:
    bool hasPathToReactor(int x, int y);

    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;
    bool mRotateLeftThrust = false;
    bool mRotateRightThrust = false;

    RotateState mRotateState = RotateState::Idle;
    qreal mRotateTargetRad = 0;
    bool mCanRotate = false;

    qreal mRotA = 0; // Rotational acceleration
    qreal mRotV = 0; // Rotational velocity
    qreal mM = 0; // Mass
    qreal mI = 0; // Inertia
    Vector mCentreOfMass = Vector(0, 0);
    Vector mCentreOfRotation = Vector(0, 0);

    QVector<std::shared_ptr<Engine>> mEngines;

    QMap<QPair<int, int>, std::shared_ptr<Component>> mComponentMap;
};
