#include "include/vector.h"
#include "include/directions.h"
#include "blur.h"
#include "include/engine.h"
#include <QGraphicsItem>
#include <QtMath>

/**
 * Physics-enabled PlayerShip graphics item
 */
class PlayerShip : public QGraphicsItem {
public:
    PlayerShip();
    enum { Type = 1 };
    int type() const override { return Type; }

    enum Component {
        Bridge,
        Thruster,
        CruiseThruster,
    };

    static QMap<Component, qreal> sComponentMass;

    /**
     * Adds the given velocity vector to the velocity
     * vector of this PlayerShip.
     * @param v vector to add
     */
    void addVelocityVector(Vector v);

    /**
     * Adds a velocity vector to the current velocity
     * based on the current direction of the ship.
     * @param s magnitude of added velocity vector
     */
    void addLongitudinalVelocity(qreal s);

    /**
     * Adds a velocity vector to the current velocity
     * based on the current direction of the ship.
     * @param s magnitude of added velocity vector
     */
    void addLateralVelocity(qreal s);

    /**
     * Adds the given scalar to the rotational velocity
     * vector of this PlayerShip.
     * @param s scalar to add
     */
    void addRotationalVelocity(qreal s);

    /**
     * Adds the given acceleration vector to the
     * acceleration vector of this PlayerShip.
     * @param v vector to add
     */
    void addAccelerationVector(Vector a);

    /**
     * Sets the given acceleration vector as the
     * acceleration vector of this PlayerShip.
     * @param v vector to add
     */
    void setAccelerationVector(Vector a);

    /**
     * Multiplies the velocity of this PlayerShip by
     * the given scalar.
     * @param scalar
     */
    void velocityMultiply(qreal scalar);

    /**
     * Adds the given scalar to the velocity
     * of this PlayerShip.
     * @param scalar
     */
    void velocityAddition(qreal scalar);

    void addBridge(int x, int y);
    void addThruster(int x, int y);
    void addCruiseEngine(int x, int y, TwoDeg direction);

    void computeRotationalInertia();
    void computeStaticForceVectors();

    bool isGridLineFree(int x, int y, TwoDeg direction);

    void computeThrusterDirectionForce(int x, int y, TwoDeg direction);
    void computeCruiseEngineDirectionForce(int x, int y, TwoDeg direction);

    void update(qreal deltaT);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    qreal getEnergy() const { return 0.5*mM*qPow(mV.getSize(), 2.0); }
    qreal getVelocity() { return mV.getSize(); }
    Vector getVelocityVector() { return mV; }
    qreal getAtan2() { return mAtan2; }

    void resetMovement();
    void enableForward() { mForwardThrust = true; }
    void enableBackward() { mBackwardThrust = true; }
    void enableLateralLeft() { mLeftThrust = true; }
    void enableLateralRight() { mRightThrust = true; }
    void enableRotateLeft() { mRotateLeftThrust = true; }
    void enableRotateRight() { mRotateRightThrust = true; }

private:
    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;
    bool mRotateLeftThrust = false;
    bool mRotateRightThrust = false;
    bool mStrafeMode = false;

    qreal mAtan2 = 0; // Bearing (rads)
    qreal mRotA = 0; // Rotational acceleration
    qreal mRotV = 0; // Rotational velocity
    Vector mV = Vector(0, 0); // Velocity vector
    Vector mA = Vector(0, 0); // Acceleration vector
    qreal mM = 0; // Mass
    qreal mI = 0; // Inertia
    Vector mCentreOfMass = Vector(0, 0);

    QVector<Engine*> mEngines;
    QVector<QPolygonF> mPolys;
    QVector<QPointF> mEngineMarkers;
    QVector<bool> mEngineEnabled;

    QMap<QPair<int, int>, Component> mComponentMap;
    QMap<QPair<int, int>, TwoDeg> mComponentDirection;

    static int const sBlockSize = 10;
    static int const sGridSize = 5;
    static int const sGridSceneSize = 5;
    constexpr static qreal const sThrusterForce = 1.0;
};