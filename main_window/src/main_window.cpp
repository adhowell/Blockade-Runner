#include "include/main_window.h"

#include <QHBoxLayout>
#include <QSplitter>


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
    setMinimumSize(gWidth, gHeight);
}

void MainWindow::closeConfigScreen()
{
    auto currLayout = layout();
    mConfigScene->getView()->hide();
    currLayout->removeWidget(mConfigScene->getView());
    delete currLayout;

    auto rightSplitter = new QSplitter;
    rightSplitter->setChildrenCollapsible(false);
    rightSplitter->setOrientation(Qt::Orientation::Vertical);
    rightSplitter->addWidget(mTacticalScene->getView());
    rightSplitter->addWidget(mTerminal);
    //mTerminal->setMinimumSize(250, 100);
    mTacticalScene->getView()->setMinimumSize(250, 250);

    auto topSplitter = new QSplitter;
    topSplitter->setChildrenCollapsible(false);
    topSplitter->addWidget(mStrategicScene->getView());
    topSplitter->addWidget(rightSplitter);
    mStrategicScene->getView()->setMinimumSize(250, 250);
    topSplitter->setStretchFactor(0, 5);
    topSplitter->setStretchFactor(1, 2);

    auto fullLayout = new QVBoxLayout;
    fullLayout->addWidget(topSplitter);
    setLayout(fullLayout);

    connect(mTerminal, &Terminal::setThrustDirection, mSimulation, &SimulationLoop::setThrust);
    connect(mTerminal, &Terminal::rotate, mSimulation, &SimulationLoop::rotate);
    connect(mTerminal, &Terminal::toggleTacticalZoom, mTacticalScene, &TacticalScene::toggleZoom);
    connect(mTerminal, &Terminal::toggleMapZoom, mStrategicScene, &StrategicScene::toggleZoom);
    mTerminal->show();
    mTerminal->setInputFocus();

    mSimulation->start();
}