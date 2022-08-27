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
/*
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
*/
StrategicScene::StrategicScene(QWidget* parent) : QGraphicsScene(parent)
{
    initBackground();
    setSceneRect(QRectF(-100, -100, 200, 200));
    mView = new StrategicView(this);
    mPlayerSymbol = new PlayerSymbolItem({0, 0});
    addItem(mPlayerSymbol);
}

void StrategicScene::updatePlayerSymbol(QPointF posOffset, qreal angle, Vector velocity)
{
    mPlayerSymbol->applyUpdate(posOffset*mScaleFactor, angle, velocity);
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

}

StrategicView* StrategicScene::getView() const
{
    return mView;
}