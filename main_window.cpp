#include "main_window.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
{
    mTerminal = new Terminal(this);
    mTacticalScene = new TacticalScene(this);
    mConfigScene = new ConfigScene(this);

    mSimulation = new SimulationLoop(mTacticalScene, mConfigScene);

    connect(mSimulation, &SimulationLoop::relayText, mTerminal, &Terminal::displayText);
    connect(mConfigScene, &ConfigScene::close, this, &MainWindow::closeConfigScreen);

    auto layout = new QHBoxLayout;
    layout->addWidget(mConfigScene->getView());
    setLayout(layout);

    setWindowTitle("Blockade Runner");
    setFixedSize(gWidth, gHeight);
}

void MainWindow::closeConfigScreen()
{
    auto currLayout = layout();
    mConfigScene->getView()->hide();
    currLayout->removeWidget(mConfigScene->getView());
    delete currLayout;

    auto newLayout = new QHBoxLayout;
    newLayout->addWidget(mTacticalScene->getView());
    newLayout->addWidget(mTerminal);
    setLayout(newLayout);

    connect(mTerminal, &Terminal::setThrustDirection, mSimulation, &SimulationLoop::setThrust);
    connect(mTerminal, &Terminal::rotate, mSimulation, &SimulationLoop::rotate);
    mTerminal->show();
    mTerminal->setInputFocus();
}