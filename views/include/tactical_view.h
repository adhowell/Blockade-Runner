#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once


class TacticalView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TacticalView(QGraphicsScene* scene);

    void toggleZoom();

private:
    bool mZoomed = true;
};

class TacticalScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TacticalScene(QWidget *parent = nullptr);

    void initBackground();
    void initAsteroidField();

    void updateItems(QPointF offset);
    TacticalView* getView() const;

public Q_SLOTS:
    void toggleZoom();

private:
    TacticalView* mView;
};


