#include "main_window.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
{
    mTerminal = new Terminal(this);
    mTacticalScene = new TacticalScene(this);

    mSimulation = new SimulationLoop(mTacticalScene);

    connect(mTerminal, &Terminal::setThrustDirection, mSimulation, &SimulationLoop::setThrust);
    connect(mTerminal, &Terminal::rotate, mSimulation, &SimulationLoop::rotate);
    connect(mSimulation, &SimulationLoop::relayText, mTerminal, &Terminal::displayText);

    auto layout = new QHBoxLayout;
    layout->addWidget(mTacticalScene->getView());
    layout->addWidget(mTerminal);
    setLayout(layout);

    setWindowTitle("Blockade Runner");
    setFixedSize(gWidth, gHeight);

    mTerminal->setInputFocus();
}