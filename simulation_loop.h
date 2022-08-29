#include "world_objects/include/player_ship.h"
#include "include/tactical_view.h"
#include "include/strategic_view.h"
#include "include/config_view.h"
#include "../global_config.h"

#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once

/**
 * Controller for linking the updates of the player ship, the world and the terminal.
 */
class SimulationLoop : public QObject
{
    Q_OBJECT
public:
    explicit SimulationLoop(TacticalScene* tacticalScene, StrategicScene* strategicScene, ConfigScene* configScene);

    void initPlayer();

    void start();
    void timerEvent(QTimerEvent* event) override;

public Q_SLOTS:
    void receiveInfoFromPlayerShip(const QString& text);
    void receiveWarningFromPlayerShip(const QString& text);
    void receiveErrorFromPlayerShip(const QString& text);
    void setThrust(TwoDeg direction, bool isActive);
    void rotate(int degrees);
    void addSensors(QVector<std::shared_ptr<Sensor>> sensors);
    void clearSensors(QVector<std::shared_ptr<Sensor>> sensors);

Q_SIGNALS:
    // For display in the terminal history window.
    void relayInfo(QString text);
    void relayWarning(QString text);
    void relayError(QString text);

private:
    PlayerShip* mPlayer;

    QVector<std::shared_ptr<WorldObject>> mObjects;

    TacticalScene* mTacticalScene;
    StrategicScene* mStrategicScene;
    ConfigScene* mConfigScene;

    void applyPlayerInput();
    void updatePlayer();

    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;

    qreal mDeltaT = 1.0;
};