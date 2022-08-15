#include "simulation_loop.h"
#include <QtWidgets>
#include <QFrame>
#include <QDebug>

SimulationLoop::SimulationLoop(TacticalScene* tacticalScene, ConfigScene* configScene) : QObject()
{
    mTacticalScene = tacticalScene;
    mConfigScene = configScene;
    initPlayer();
    startTimer(1000/gTargetFramerate);
}

void SimulationLoop::initPlayer()
{
    mPlayer = new PlayerShip();
    connect(mPlayer, &PlayerShip::displayText, this, &SimulationLoop::receiveTextFromPlayerShip);
    connect(mPlayer, &PlayerShip::handleAddConfigComponent, mConfigScene, &ConfigScene::drawConfigComponent);
    connect(mPlayer, &PlayerShip::handleAddConfigEngine, mConfigScene, &ConfigScene::drawConfigEngine);
    connect(mPlayer, &PlayerShip::handleRemoveAllConfigItems, mConfigScene, &ConfigScene::deleteAllComponents);
    connect(mConfigScene->getView(), &ConfigView::addShipPart, mPlayer, &PlayerShip::handleAddPart);

    /*
    mPlayer->addRotateThruster(0, 0);
    mPlayer->addRotateThruster(0, 4);
    mPlayer->addRotateThruster(4, 0);
    mPlayer->addRotateThruster(4, 4);
    mPlayer->addCruiseThruster(2, 0, TwoDeg::Down);
    mPlayer->addCruiseThruster(2, 4, TwoDeg::Up);
    mPlayer->addCargo(1, 0);
    mPlayer->addCargo(1, 1);
    mPlayer->addCargo(1, 2);
    mPlayer->addCargo(1, 3);
    mPlayer->addCargo(1, 4);
    mPlayer->addCargo(3, 0);
    mPlayer->addCargo(3, 1);
    mPlayer->addCargo(3, 2);
    mPlayer->addCargo(3, 3);
    mPlayer->addHeatSink(3, 4);
    mPlayer->computeStaticForceVectors();
    */

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