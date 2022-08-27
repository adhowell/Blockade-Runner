#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once


class TacticalView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TacticalView(QGraphicsScene* scene);

    void wheelEvent(QWheelEvent* event) override;

private:
    int mScaleFactor = 3;
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

private:
    TacticalView* mView;
};


