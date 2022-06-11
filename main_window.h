#include "views/player_view.h"
#include "include/terminal.h"
#include <QGraphicsScene>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    QGraphicsScene* mScene;
    Terminal* mTerminal;
    View* mShipView;
};