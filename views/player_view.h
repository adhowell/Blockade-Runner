#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QGraphicsScene* scene);

    void wheelEvent(QWheelEvent* event) override;

private:
    int mScaleFactor = 3;
};

class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(QGraphicsScene* scene, QWidget *parent = nullptr);

    GraphicsView* getGraphicsView() const;

private:
    GraphicsView* mGraphicsView;
};


