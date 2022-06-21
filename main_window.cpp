#include "main_window.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
{
    mTerminal = new Terminal(this);

    mScene = new SimulationLoop(this);
    mShipView = new View(mScene);

    connect(mTerminal, &Terminal::setThrustDirection, mScene, &SimulationLoop::setThrust);
    connect(mScene, &SimulationLoop::relayText, mTerminal, &Terminal::displayText);

    auto layout = new QHBoxLayout;
    layout->addWidget(mShipView);
    layout->addWidget(mTerminal);
    setLayout(layout);

    setWindowTitle("Blockade Runner");
    setFixedSize(gWidth, gHeight);
}