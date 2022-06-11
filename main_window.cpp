#include "main_window.h"
#include "global_config.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
{
    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(QRectF(-100, -100, 200, 200));

    mShipView = new View();
    mShipView->getQGraphicsView()->setScene(mScene);
    mShipView->getGraphicsView()->initBackground();
    mShipView->getGraphicsView()->initPlayer();
    //mView->getGraphicsView()->initAsteroidField();

    mTerminal = new Terminal(this);

    connect(mTerminal, &Terminal::setThrustDirection, mShipView, &View::setThrust);

    auto layout = new QHBoxLayout;
    layout->addWidget(mShipView);
    layout->addWidget(mTerminal);
    setLayout(layout);

    setWindowTitle("Blockade Runner");
    setFixedSize(gWidth, gHeight);

    mShipView->getGraphicsView()->startTimer(1000/gTargetFramerate);
}