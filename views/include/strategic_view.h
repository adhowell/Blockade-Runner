#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#include "../objects/include/player_symbol_item.h"
#include "../objects/include/grid_lines.h"
#include "../objects/include/velocity_marker.h"

#pragma once


class StrategicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit StrategicView(QGraphicsScene* scene);

    void wheelEvent(QWheelEvent* event) override;

private:
    int mScaleFactor = 3;
};

class StrategicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit StrategicScene(QWidget *parent = nullptr);

    void initBackground();

    void updatePlayer(QPointF posDelta, qreal angleDelta, Vector vel, Vector acc);
    StrategicView* getView() const;

private:
    StrategicView* mView;
    PlayerSymbolItem* mPlayerSymbol;
    GridLines* mGridLines;
    QVector<VelocityMarker*> mVelMarkers;
};


