#include "include/player_ship.h"
#include "include/asteroid.h"
#include "include/starfield.h"
#include "include/phosphor_ghost.h"
#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View *v);

    void initBackground();
    void initPlayer();
    void initAsteroidField();

    void setThrust(TwoDeg direction, bool isActive);

    void timerEvent(QTimerEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    View *mView;
    PlayerShip* mPlayer;

    void applyPlayerInput();
    void updatePlayer();

    bool mForwardThrust = false;
    bool mBackwardThrust = false;
    bool mLeftThrust = false;
    bool mRightThrust = false;
    bool mStrafeMode = false;

    qreal mDeltaT = 1.0;
    int mScaleFactor = 3;
};

class View : public QFrame {
Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);

    QGraphicsView* getQGraphicsView() const;
    GraphicsView* getGraphicsView() const;

public Q_SLOTS:
    void setThrust(TwoDeg direction, bool isActive);

private:
    GraphicsView* mGraphicsView;
};
