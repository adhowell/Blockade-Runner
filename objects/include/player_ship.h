#include "include/vector.h"
#include "blur.h"
#include "include/engine.h"
#include "include/component.h"
#include "player_ship_item.h"

#include <QGraphicsItem>
#include <QtMath>

#pragma once

class PlayerShip : public QObject {
    Q_OBJECT
public:
    PlayerShip();

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
    void computeStaticForceVectors();
    void computeCentreOfRotation();
    void computeProperties();
    void reconfigure();
    void updateVisuals();

    bool isGridLineFree(int x, int y, TwoDeg direction);

    template<class T>
    void computeEngineDirectionForce(int x, int y, TwoDeg direction);

    void update(qreal deltaT);

    qreal getEnergy() const { return 0.5*mM*qPow(mV.getSize(), 2.0); }
    qreal getVelocity() { return mV.getSize(); }
    Vector getVelocityVector() { return mV; }
    qreal getAtan2() { return mAtan2; }

    QGraphicsItem* getTacticalGraphicsItem() { return mTacticalGraphicsItem; }

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

    qreal mAtan2 = 0; // Bearing (rads)
    qreal mRotA = 0; // Rotational acceleration
    qreal mRotV = 0; // Rotational velocity
    Vector mV = Vector(0, 0); // Velocity vector
    Vector mA = Vector(0, 0); // Acceleration vector
    qreal mM = 0; // Mass
    qreal mI = 0; // Inertia
    Vector mCentreOfMass = Vector(0, 0);
    Vector mCentreOfRotation = Vector(0, 0);

    PlayerShipItem* mTacticalGraphicsItem;
    QVector<std::shared_ptr<Engine>> mEngines;

    QMap<QPair<int, int>, std::shared_ptr<Component>> mComponentMap;
};
