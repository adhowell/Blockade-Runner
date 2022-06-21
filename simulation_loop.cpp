#include "simulation_loop.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

SimulationLoop::SimulationLoop(QWidget* parent) : QGraphicsScene(parent)
{
    initBackground();
    initPlayer();
    startTimer(1000/gTargetFramerate);
    setSceneRect(QRectF(-100, -100, 200, 200));
}

void SimulationLoop::initBackground()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(0, 0, 15));
    addRect(-1000, -700, 2000, 1400, QPen(), brush);

    auto starField = new Starfield(QPointF(0, 0));
    addItem(starField);
}

void SimulationLoop::initPlayer()
{
    mPlayer = new PlayerShip();
    connect(mPlayer, &PlayerShip::displayText, this, &SimulationLoop::receiveTextFromPlayerShip);

    mPlayer->addThruster(0, 0);
    mPlayer->addThruster(0, 4);
    mPlayer->addThruster(4, 0);
    mPlayer->addThruster(4, 4);
    mPlayer->addCruiseEngine(2, 3, TwoDeg::Up);
    mPlayer->computeStaticForceVectors();
    addItem(mPlayer->getGraphicsItem());
}

void SimulationLoop::initAsteroidField()
{
    auto asteroid = new Asteroid(QColor(0, 255, 0), 0, -200, Vector(0, 0), 10, 25);
    addItem(asteroid);
}

void SimulationLoop::timerEvent(QTimerEvent *event)
{
    applyPlayerInput();
    Vector playerVelocity = mPlayer->getVelocityVector();
    playerVelocity.flip();
    QPointF playerOffset = playerVelocity.getPosDelta(mDeltaT);

    QList<QGraphicsItem*> forDeletion;
    for (QGraphicsItem *item: items()) {
        if (Asteroid *a = dynamic_cast<Asteroid*>(item)) {
            a->posUpdate(playerOffset);
            a->update();
        }
        if (PhosphorGhost *g = dynamic_cast<PhosphorGhost*>(item)) {
            if (g->isDone()) forDeletion << g;
            g->posUpdate(playerOffset);
            g->update();
        }
        if (Starfield *s = dynamic_cast<Starfield*>(item)) {
            s->updateOffset(playerOffset);
            s->update();
        }
    }

    for (auto i : forDeletion) removeItem(i);

    //auto ghost = new PhosphorGhost(mPlayer->getPoly(), 20);
    //scene()->addItem(ghost);
}

void SimulationLoop::applyPlayerInput()
{
    mPlayer->resetMovement();
    if (mLeftThrust)
        mPlayer->enableRotateLeft();
    if (mRightThrust)
        mPlayer->enableRotateRight();
    if (mForwardThrust)
        mPlayer->enableForward();
    if (mBackwardThrust)
        mPlayer->enableBackward();

    updatePlayer();
}

void SimulationLoop::updatePlayer()
{
    mPlayer->update(mDeltaT);
}

void SimulationLoop::setThrust(TwoDeg direction, bool isActive)
{
    switch (direction)
    {
        case TwoDeg::Up:
            mForwardThrust = isActive;
            break;
        case TwoDeg::Down:
            mBackwardThrust = isActive;
            break;
        case TwoDeg::Left:
            mLeftThrust = isActive;
            break;
        case TwoDeg::Right:
            mRightThrust = isActive;
            break;
    }
}

void SimulationLoop::receiveTextFromPlayerShip(const QString& text)
{
    Q_EMIT relayText(text);
}