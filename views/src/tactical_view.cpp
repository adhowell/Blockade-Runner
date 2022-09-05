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
    setStyleSheet("border: 1px solid green");
    setScene(scene);
    ensureVisible(QRectF(0, 0, 0, 0));
}

void TacticalView::toggleZoom()
{
    if (mZoomed) {
        scale(0.5, 0.5);
    } else {
        scale(1.0/0.5, 1.0/0.5);
    }
    mZoomed = !mZoomed;
}

TacticalScene::TacticalScene(QWidget* parent) : QGraphicsScene(parent)
{
    initBackground();
    setSceneRect(QRectF(-100, -100, 200, 200));
    mView = new TacticalView(this);
}

void TacticalScene::updateItems(QPointF offset)
{
    QList<QGraphicsItem*> forDeletion;
    for (QGraphicsItem *item: items()) {
        if (auto a = dynamic_cast<Asteroid*>(item))
        {
            a->posUpdate(offset);
            a->update();
        }
        else if (auto g = dynamic_cast<PhosphorGhost*>(item))
        {
            if (g->isDone()) forDeletion << g;
            g->posUpdate(offset);
            g->update();
        }
        else if (auto s = dynamic_cast<Starfield*>(item))
        {
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
    setBackgroundBrush(QColor(0, 0, 15));

    addItem(new Starfield(QPointF(0, 0), 0.3, 17));
    addItem(new Starfield(QPointF(0, 0), 0.5, 35));
    addItem(new Starfield(QPointF(0, 0), 1.0, 29));
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

void TacticalScene::toggleZoom()
{
    mView->toggleZoom();
}