#include "include/player_ship.h"
#include "include/tactical_view.h"
#include "../global_config.h"
#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once

class SimulationLoop : public QObject
{
    Q_OBJECT
public:
    explicit SimulationLoop(TacticalScene* tacticalScene);

    void initPlayer();

    void timerEvent(QTimerEvent* event) override;

public Q_SLOTS:
    void receiveTextFromPlayerShip(const QString& text);
    void setThrust(TwoDeg direction, bool isActive);
    void rotate(int degrees);

Q_SIGNALS:
    /**
     * For display in the terminal history window.
     */
    void relayText(QString text);

private:
    PlayerShip* mPlayer;

    TacticalScene* mTacticalScene;

    void applyPlayerInput();
    void updatePlayer();

    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;

    qreal mDeltaT = 1.0;
};