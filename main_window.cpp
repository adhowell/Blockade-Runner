#include "main_window.h"
#include "global_config.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
{
    mTerminal = new Terminal(this);

    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(QRectF(-100, -100, 200, 200));
    mShipView = new View();
    mShipView->getQGraphicsView()->setScene(mScene);
    connect(mTerminal, &Terminal::setThrustDirection, mShipView, &View::setThrust);
    connect(mShipView->getGraphicsView(), &GraphicsView::relayText, mTerminal, &Terminal::displayText);

    auto layout = new QHBoxLayout;
    layout->addWidget(mShipView);
    layout->addWidget(mTerminal);
    setLayout(layout);

    setWindowTitle("Blockade Runner");
    setFixedSize(gWidth, gHeight);

    mShipView->getGraphicsView()->initBackground();
    mShipView->getGraphicsView()->initPlayer();
    mShipView->getGraphicsView()->startTimer(1000/gTargetFramerate);
}