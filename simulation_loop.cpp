#include "simulation_loop.h"
#include <QtWidgets>
#include <QFrame>
#include <QDebug>

SimulationLoop::SimulationLoop(TacticalScene* tacticalScene, StrategicScene* strategicScene, ConfigScene* configScene) : QObject()
{
    mTacticalScene = tacticalScene;
    mStrategicScene = strategicScene;
    mConfigScene = configScene;
    initPlayer();
    startTimer(1000/gTargetFramerate);
}

void SimulationLoop::initPlayer()
{
    mPlayer = new PlayerShip();
    connect(mPlayer, &PlayerShip::displayText, this, &SimulationLoop::receiveInfoFromPlayerShip);
    connect(mPlayer, &PlayerShip::handleAddConfigComponent, mConfigScene, &ConfigScene::drawConfigComponent);
    connect(mPlayer, &PlayerShip::handleAddConfigEngine, mConfigScene, &ConfigScene::drawConfigEngine);
    connect(mPlayer, &PlayerShip::handleAddCentreOfMass, mConfigScene, &ConfigScene::drawCentreOfMass);
    connect(mPlayer, &PlayerShip::handleAddCentreOfRotation, mConfigScene, &ConfigScene::drawCentreOfRotation);
    connect(mPlayer, &PlayerShip::handleRemoveAllConfigItems, mConfigScene, &ConfigScene::deleteAllComponents);
    connect(mConfigScene->getView(), &ConfigView::addShipPart, mPlayer, &PlayerShip::handleAddPart);
    connect(mConfigScene->getView(), &ConfigView::removeShipPart, mPlayer, &PlayerShip::handleRemovePart);

    mPlayer->handleAddPart(Component::ComponentType::Reactor, {2, 2}, TwoDeg::Up);

    mTacticalScene->addItem(mPlayer->getTacticalGraphicsItem());
}

void SimulationLoop::timerEvent(QTimerEvent *event)
{
    // In the tactical scene the player is always at the origin, the world moves instead
    applyPlayerInput();
    Vector playerVelocity = mPlayer->getVelocityVector();
    playerVelocity.flip();
    QPointF playerOffset = playerVelocity.getPosDelta(mDeltaT);
    mTacticalScene->updateItems(playerOffset);

    mStrategicScene->updatePlayerSymbol(-playerOffset, mPlayer->getAtan2(), mPlayer->getVelocityVector());
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

void SimulationLoop::receiveInfoFromPlayerShip(const QString& text)
{
    Q_EMIT relayInfo(text);
}

void SimulationLoop::receiveWarningFromPlayerShip(const QString& text)
{
    Q_EMIT relayWarning(text);
}

void SimulationLoop::receiveErrorFromPlayerShip(const QString& text)
{
    Q_EMIT relayError(text);
}