#include "include/world_object.h"
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
    PlayerShip(Faction faction, uint32_t uid);

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
    void addSensor(int x, int y, TwoDeg direction);

    void computeRotationalInertia();

    /**
     * Iterates over every component in the ship and creates the
     * associated engines and sensors.
     */
    void createAllSubComponents();

    /**
     * Add a sensor to the player ship based on the component position
     * and type.
     *
     * @param c - The component to use for adding the sensor.
     */
    void createComponentSensors(std::shared_ptr<Component>& c);

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

    bool isGridLineFree(int x, int y, TwoDeg direction, bool flip = false);

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
    void displayText(QString);
    void handleAddConfigComponent(std::shared_ptr<Component>);
    void handleAddConfigEngine(std::shared_ptr<Engine>);
    void handleAddCentreOfMass(qreal, qreal);
    void handleAddCentreOfRotation(qreal, qreal);
    void handleRemoveAllConfigItems();
    void handleClearSensors(QVector<std::shared_ptr<Sensor>>);

private:
    bool hasPathToReactor(int x, int y);

    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;
    bool mRotateLeftThrust = false;
    bool mRotateRightThrust = false;

    bool mCanRotate = false;

    qreal mM = 0; // Mass
    qreal mI = 0; // Inertia
    Vector mCentreOfMass = Vector(0, 0);
    Vector mCentreOfRotation = Vector(0, 0);

    QVector<std::shared_ptr<Engine>> mEngines;
    QMap<QPair<int, int>, std::shared_ptr<Component>> mComponentMap;
};
