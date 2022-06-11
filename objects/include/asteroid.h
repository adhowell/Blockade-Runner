#include "include/vector.h"
#include <QGraphicsItem>
#include <QtMath>

/**
 * Physics-enabled Asteroid graphics item
 */
class Asteroid : public QGraphicsItem
{
public:
    Asteroid(QColor color, qreal x, qreal y, Vector v, qreal mass, qreal radius);
    Asteroid(QColor color, QPointF p, Vector v, qreal mass, qreal radius);
    enum { Type = 2 };
    int type() const override { return Type; }

    /**
     * Update the position of the Asteroid given the contained
     * velocity and direction vector, and the given delta-t.
     * @param deltaT time-step used to calculate delta-pos
     */
    void advance(qreal deltaT);

    /**
     * Move the Asteroid by the given radial and distance.
     * @param dist distance to move the Asteroid by
     * @param rad radial (radians) to move along
     */
    void posUpdate(qreal dist, qreal rad);

    /**
     * Move the Asteroid by the given offset.
     * @param posDelta offset to apply to current position
     */
    void posUpdate(QPointF posDelta);

    /**
     * Update the radial direction the Asteroid is moving along
     * by reflecting the current travel vector about a vector
     * with the given radial.
     * @param rad radial (radians) to flip about.
     */
    void vectorReflect(qreal rad);

    /**
     * Updates the velocity vector of this object and
     * of the given other Asteroid, based on an elastic
     * collision.
     * @param b Asteroid object this Asteroid is colliding with
     */
    void collide(Asteroid* b);

    /**
     * Return the radial angle of the vector between the
     * centre of this Asteroid to the centre of the given
     * Asteroid (in that direction).
     * @param b Other Asteroid to calculate angle to
     */
    qreal atan2(Asteroid* b) const;

    /**
     * Return the distance from the centre of this Asteroid
     * to the centre of the given Asteroid.
     * @param b Other Asteroid to calculate distance to
     */
    qreal distance(Asteroid* b) const;

    /**
     * Adds the given velocity vector to the velocity
     * vector of this Asteroid.
     * @param v vector to add
     */
    void addVelocityVector(Vector v);

    /**
     * Adds the given acceleration vector to the
     * acceleration vector of this Asteroid.
     * @param v vector to add
     */
    void addAccelerationVector(Vector a);

    /**
     * Sets the given acceleration vector as the
     * acceleration vector of this Asteroid.
     * @param v vector to add
     */
    void setAccelerationVector(Vector a);

    /**
     * Multiplies the velocity of this Asteroid by
     * the given scalar.
     * @param scalar
     */
    void velocityMultiply(qreal scalar);

    /**
     * Adds the given scalar to the velocity
     * of this Asteroid.
     * @param scalar
     */
    void velocityAddition(qreal scalar);

    void storePosition() { mOldP = mP; }

    void setTimeRemaining(qreal t) { mTimeRemaining = t; }

    void decrementTime(qreal deltaT) { mTimeRemaining -= deltaT; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void update() { setPos(mP); }

    qreal getRadius() const { return mR; }
    qreal getDeltaPos() const { return qSqrt(qPow(mP.x()-mOldP.x(), 2.0) + qPow(mP.y()-mOldP.y(), 2.0)); }
    qreal getEnergy() const { return 0.5*mM*qPow(mV.getSize(), 2.0); }
    qreal getTimeRemaining() { return mTimeRemaining; }
    qreal getVelocity() { return mV.getSize(); }
    QPointF getPos() { return mP; }

    static qreal sMinMass;
    static qreal sMaxMass;
    static qreal sMinRadius;
    static qreal sMaxRadius;

private:
    QColor mColor;
    QPointF mP; // Centre point
    Vector mV;  // Velocity vector
    Vector mA;  // Acceleration vector
    qreal mM;   // Mass
    qreal mR;   // Radius
    QPointF mOldP;  // Position at start of sub-epoch
    qreal mTimeRemaining; // For current sub-epoch
};