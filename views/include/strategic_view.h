#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#include "../objects/include/player_symbol_item.h"

#pragma once


class StrategicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit StrategicView(QGraphicsScene* scene);

    //void wheelEvent(QWheelEvent* event) override;

private:
    int mScaleFactor = 3;
};

class StrategicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit StrategicScene(QWidget *parent = nullptr);

    void initBackground();

    void updatePlayerSymbol(QPointF posDelta, qreal angleDelta, Vector velocity);
    StrategicView* getView() const;

private:
    StrategicView* mView;
    PlayerSymbolItem* mPlayerSymbol;
    qreal mScaleFactor = 0.0005;
};


