#include "simulation_loop.h"
#include "models/include/radar_sensor.h"

#include <QtWidgets>
#include <QFrame>
#include <QDebug>

SimulationLoop::SimulationLoop(TacticalScene* tacticalScene, StrategicScene* strategicScene, ConfigScene* configScene) : QObject()
{
    mTacticalScene = tacticalScene;
    mStrategicScene = strategicScene;
    mConfigScene = configScene;
    initPlayer();
}

void SimulationLoop::start()
{
    startTimer(1000/gTargetFramerate);
}

void SimulationLoop::initPlayer()
{
    mPlayer = new PlayerShip(WorldObject::Faction::Blue, 0);
    connect(mPlayer, &PlayerShip::displayText, this, &SimulationLoop::receiveInfoFromPlayerShip);
    connect(mPlayer, &PlayerShip::handleAddConfigComponent, mConfigScene, &ConfigScene::drawConfigComponent);
    connect(mPlayer, &PlayerShip::handleAddConfigEngine, mConfigScene, &ConfigScene::drawConfigEngine);
    connect(mPlayer, &PlayerShip::handleAddCentreOfMass, mConfigScene, &ConfigScene::drawCentreOfMass);
    connect(mPlayer, &PlayerShip::handleAddCentreOfRotation, mConfigScene, &ConfigScene::drawCentreOfRotation);
    connect(mPlayer, &PlayerShip::handleRemoveAllConfigItems, mConfigScene, &ConfigScene::deleteAllComponents);
    connect(mPlayer, &PlayerShip::handleCreatePlayerSensor, this, &SimulationLoop::createPlayerSensor);
    connect(mPlayer, &PlayerShip::handleClearSensors, this, &SimulationLoop::clearSensors);
    connect(mConfigScene->getView(), &ConfigView::addShipPart, mPlayer, &PlayerShip::handleAddPart);
    connect(mConfigScene->getView(), &ConfigView::removeShipPart, mPlayer, &PlayerShip::handleRemovePart);

    mPlayer->handleAddPart(Component::ComponentType::Reactor, {2, 2}, TwoDeg::Up);

    mTacticalScene->addItem(mPlayer->getTacticalGraphicsItem());
}

void SimulationLoop::timerEvent(QTimerEvent *event)
{
    // The player ship is always at the origin, the world moves instead
    applyPlayerInput();
    Vector playerVelocity = mPlayer->getVelVector();
    playerVelocity.flip();
    QPointF playerOffset = playerVelocity.getPosDelta(mDeltaT);

    // Update sensors
    for (auto sensor : mSensors)
    {
        sensor->update();
    }

    mTacticalScene->updateItems(playerOffset);
    mStrategicScene->updatePlayer(playerOffset, mPlayer->getAtan2(), mPlayer->getVelVector(), mPlayer->getAccVector());
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

void SimulationLoop::createPlayerSensor(TwoDeg direction)
{
    qreal angle = 0;
    switch (direction)
    {
        case TwoDeg::Up: break;
        case TwoDeg::Right: angle = M_PI * 1.5; break;
        case TwoDeg::Down: angle = M_PI; break;
        case TwoDeg::Left: angle = M_PI * 0.5; break;
    }
    auto sensor = createSensor(mPlayer, angle);
    mStrategicScene->addItem(sensor->getItem());
    mSensors << sensor;
}

std::shared_ptr<Sensor> SimulationLoop::createSensor(WorldObject* parent, qreal boreOffsetAngle)
{
    return std::make_shared<RadarSensor>(parent, boreOffsetAngle);
}

void SimulationLoop::clearSensors()
{
    for (const auto& s : mSensors)
    {
        mStrategicScene->removeItem(s->getItem());
    }
    mSensors.clear();
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