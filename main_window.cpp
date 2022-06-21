#include "main_window.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
{
    mTerminal = new Terminal(this);
    mTacticalScene = new TacticalScene(this);

    mScene = new SimulationLoop(mTacticalScene);

    connect(mTerminal, &Terminal::setThrustDirection, mScene, &SimulationLoop::setThrust);
    connect(mScene, &SimulationLoop::relayText, mTerminal, &Terminal::displayText);

    auto layout = new QHBoxLayout;
    layout->addWidget(mTacticalScene->getView());
    layout->addWidget(mTerminal);
    setLayout(layout);

    setWindowTitle("Blockade Runner");
    setFixedSize(gWidth, gHeight);
}