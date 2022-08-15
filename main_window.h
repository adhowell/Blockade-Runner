#include "include/tactical_view.h"
#include "include/config_view.h"
#include "include/terminal.h"
#include "simulation_loop.h"
#include <QGraphicsScene>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

public Q_SLOTS:
    void closeConfigScreen();

private:
    SimulationLoop* mSimulation;
    Terminal* mTerminal;
    TacticalScene* mTacticalScene;
    ConfigScene* mConfigScene;
};