#include "player_view.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

GraphicsView::GraphicsView(View *v) : QGraphicsView(), mView(v)
{
    setMouseTracking(true);
}

void GraphicsView::initBackground()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(0, 0, 15));
    scene()->addRect(-1000, -700, 2000, 1400, QPen(), brush);

    auto starField = new Starfield(QPointF(0, 0));
    scene()->addItem(starField);
}

void GraphicsView::initPlayer()
{
    mPlayer = new PlayerShip();
    mPlayer->addThruster(0, 0);
    mPlayer->addThruster(0, 4);
    mPlayer->addThruster(4, 0);
    mPlayer->addThruster(4, 4);
    mPlayer->addCruiseEngine(2, 3, TwoDeg::Up);
    mPlayer->computeStaticForceVectors();
    scene()->addItem(mPlayer);
}

void GraphicsView::initAsteroidField()
{
    auto asteroid = new Asteroid(QColor(0, 255, 0), 0, -200, Vector(0, 0), 10, 25);
    scene()->addItem(asteroid);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        if (mScaleFactor > 0) {
            scale(1.1, 1.1);
            mScaleFactor--;
        }
    } else {
        if (mScaleFactor < 10) {
            scale(0.91, 0.91);
            mScaleFactor++;
        }
    }
}

void GraphicsView::timerEvent(QTimerEvent *event)
{
    applyPlayerInput();
    Vector playerVelocity = mPlayer->getVelocityVector();
    playerVelocity.flip();
    QPointF playerOffset = playerVelocity.getPosDelta(mDeltaT);

    QList<QGraphicsItem*> forDeletion;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item: items) {
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

    for (auto i : forDeletion) scene()->removeItem(i);

    //auto ghost = new PhosphorGhost(mPlayer->getPoly(), 20);
    //scene()->addItem(ghost);
}

void GraphicsView::applyPlayerInput()
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

void GraphicsView::updatePlayer()
{
    mPlayer->update(mDeltaT);
}

void GraphicsView::setThrust(TwoDeg direction, bool isActive)
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

View::View(QWidget* parent) : QFrame(parent)
{
    mGraphicsView = new GraphicsView(this);
    mGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(mGraphicsView, 0, 0);
    setLayout(layout);
}

QGraphicsView* View::getQGraphicsView() const
{
    return static_cast<QGraphicsView *>(mGraphicsView);
}

GraphicsView* View::getGraphicsView() const
{
    return mGraphicsView;
}

void View::setThrust(TwoDeg direction, bool isActive)
{
    mGraphicsView->setThrust(direction, isActive);
}