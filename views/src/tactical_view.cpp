#include "include/tactical_view.h"
#include "include/starfield.h"
#include "include/asteroid.h"
#include "include/phosphor_ghost.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

TacticalView::TacticalView(QGraphicsScene* scene) : QGraphicsView()
{
    setScene(scene);
    ensureVisible(QRectF(0, 0, 0, 0));
}

void TacticalView::wheelEvent(QWheelEvent *event)
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

TacticalScene::TacticalScene(QWidget* parent) : QGraphicsScene(parent)
{
    initBackground();
    initAsteroidField();
    setSceneRect(QRectF(-100, -100, 200, 200));
    mView = new TacticalView(this);
}

void TacticalScene::updateItems(QPointF offset)
{
    QList<QGraphicsItem*> forDeletion;
    for (QGraphicsItem *item: items()) {
        if (Asteroid *a = dynamic_cast<Asteroid*>(item)) {
            a->posUpdate(offset);
            a->update();
        }
        if (PhosphorGhost *g = dynamic_cast<PhosphorGhost*>(item)) {
            if (g->isDone()) forDeletion << g;
            g->posUpdate(offset);
            g->update();
        }
        if (Starfield *s = dynamic_cast<Starfield*>(item)) {
            s->updateOffset(offset);
            s->update();
        }
    }

    for (auto i : forDeletion) removeItem(i);

    //auto ghost = new PhosphorGhost(mPlayer->getPoly(), 20);
    //scene()->addItem(ghost);
}

void TacticalScene::initBackground()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(0, 0, 15));
    addRect(-1000, -700, 2000, 1400, QPen(), brush);

    auto starField = new Starfield(QPointF(0, 0));
    addItem(starField);
}

void TacticalScene::initAsteroidField()
{
    auto asteroid = new Asteroid(QColor(0, 255, 0), 0, -200, Vector(0, 0), 10, 25);
    addItem(asteroid);
}

TacticalView* TacticalScene::getView() const
{
    return mView;
}