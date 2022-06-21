#include "include/player_ship.h"
#include "include/asteroid.h"
#include "include/starfield.h"
#include "include/phosphor_ghost.h"
#include "../global_config.h"
#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once

class SimulationLoop : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SimulationLoop(QWidget* parent = nullptr);

    void initBackground();
    void initPlayer();
    void initAsteroidField();

    void timerEvent(QTimerEvent* event) override;

public Q_SLOTS:
    void receiveTextFromPlayerShip(const QString& text);
    void setThrust(TwoDeg direction, bool isActive);

Q_SIGNALS:
    /**
     * For display in the terminal history window.
     */
    void relayText(QString text);

private:
    PlayerShip* mPlayer;

    void applyPlayerInput();
    void updatePlayer();

    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;

    qreal mDeltaT = 1.0;
};