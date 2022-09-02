#include "include/main_window.h"

#include <QHBoxLayout>


MainWindow::MainWindow(QWidget* parent)
{
    setStyleSheet("color: #00ff00; background-color: black;");
    mTerminal = new Terminal(this);
    mTacticalScene = new TacticalScene(this);
    mStrategicScene = new StrategicScene(this);
    mConfigScene = new ConfigScene(this);

    mSimulation = new SimulationLoop(mTacticalScene, mStrategicScene, mConfigScene);

    connect(mSimulation, &SimulationLoop::relayInfo, mTerminal, &Terminal::displayInfo);
    connect(mSimulation, &SimulationLoop::relayWarning, mTerminal, &Terminal::displayWarning);
    connect(mSimulation, &SimulationLoop::relayError, mTerminal, &Terminal::displayError);
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

    auto rightLayout = new QVBoxLayout;
    rightLayout->addWidget(mTacticalScene->getView());
    rightLayout->addWidget(mTerminal);

    auto fullLayout = new QHBoxLayout;
    fullLayout->addWidget(mStrategicScene->getView(), 2);
    fullLayout->addLayout(rightLayout, 1);
    setLayout(fullLayout);

    connect(mTerminal, &Terminal::setThrustDirection, mSimulation, &SimulationLoop::setThrust);
    connect(mTerminal, &Terminal::rotate, mSimulation, &SimulationLoop::rotate);
    mTerminal->show();
    mTerminal->setInputFocus();

    mSimulation->start();
}