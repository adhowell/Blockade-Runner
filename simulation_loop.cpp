#include "simulation_loop.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

SimulationLoop::SimulationLoop(TacticalScene* tacticalScene) : QObject()
{
    mTacticalScene = tacticalScene;
    initPlayer();
    startTimer(1000/gTargetFramerate);
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

    mTacticalScene->addItem(mPlayer->getTacticalGraphicsItem());
}

void SimulationLoop::timerEvent(QTimerEvent *event)
{
    applyPlayerInput();
    Vector playerVelocity = mPlayer->getVelocityVector();
    playerVelocity.flip();
    QPointF playerOffset = playerVelocity.getPosDelta(mDeltaT);
    mTacticalScene->updateItems(playerOffset);
}

void SimulationLoop::applyPlayerInput()
{
    mPlayer->resetMovement();
    //if (mLeftThrust)
    //    mPlayer->enableRotateLeft();
    //if (mRightThrust)
    //    mPlayer->enableRotateRight();
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

void SimulationLoop::rotate(int degrees)
{
    mPlayer->rotate(degrees);
}

void SimulationLoop::receiveTextFromPlayerShip(const QString& text)
{
    Q_EMIT relayText(text);
}