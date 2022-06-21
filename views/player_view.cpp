#include "player_view.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

GraphicsView::GraphicsView(QGraphicsScene* scene) : QGraphicsView()
{
    setScene(scene);
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

View::View(QGraphicsScene* scene, QWidget* parent) : QFrame(parent)
{
    mGraphicsView = new GraphicsView(scene);
    mGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    auto layout = new QGridLayout;
    layout->addWidget(mGraphicsView, 0, 0);
    setLayout(layout);
}

GraphicsView* View::getGraphicsView() const
{
    return mGraphicsView;
}