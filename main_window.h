#include "include/tactical_view.h"
#include "include/terminal.h"
#include "simulation_loop.h"
#include <QGraphicsScene>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    SimulationLoop* mScene;
    Terminal* mTerminal;
    TacticalScene* mTacticalScene;
};