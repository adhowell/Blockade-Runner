#include "include/strategic_view.h"

#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

StrategicView::StrategicView(QGraphicsScene* scene) : QGraphicsView()
{
    setStyleSheet("border: 1px solid green");
    setScene(scene);
    ensureVisible(QRectF(0, 0, 0, 0));
}

void StrategicView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
    {
        scale(1.1, 1.1);
    } else
    {
        scale(0.91, 0.91);
    }
}

StrategicScene::StrategicScene(QWidget* parent) : QGraphicsScene(parent)
{
    initBackground();
    setSceneRect(QRectF(-100, -100, 200, 200));
    mView = new StrategicView(this);
    mPlayerSymbol = new PlayerSymbolItem({0, 0});
    addItem(mPlayerSymbol);
}

void StrategicScene::updatePlayer(QPointF posOffset, qreal angle, Vector velocity)
{
    posOffset *= mScaleFactor;
    mPlayerSymbol->applyUpdate(angle, velocity);
    mGridLines->updateOffset(posOffset);
}
/*
void StrategicScene::updateItems(QPointF offset)
{
    QList<QGraphicsItem*> forDeletion;
    for (QGraphicsItem *item: items()) {
        if (Asteroid *a = dynamic_cast<Asteroid*>(item)) {
            a->posUpdate(offset);
            a->update();
        }
        if (PhosphorGhost *g = dynamic_cast<PhosphorGhost*>(item)) {
            if (g->isDone()) forDeletion << g;
            g->applyPosOffset(offset);
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
*/
void StrategicScene::initBackground()
{
    setBackgroundBrush(QColor(0, 0, 15));
    mGridLines = new GridLines(200);
    addItem(mGridLines);
}

StrategicView* StrategicScene::getView() const
{
    return mView;
}