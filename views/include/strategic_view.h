#include "include/player_symbol_item.h"
#include "include/grid_lines.h"
#include "include/velocity_marker.h"
#include "include/acceleration_marker.h"
#include "include/signal_track_processor.h"
#include "include/strategic_symbol.h"

#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>

#pragma once


class StrategicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit StrategicView(QGraphicsScene* scene);

    void toggleZoom();

private:
    bool mZoomed = true;
};

class StrategicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit StrategicScene(QWidget *parent = nullptr);

    void initBackground();

    void visualiseTracks(const QVector<SignalTrackProcessor::Track>& tracks);
    void updateTrack(SignalTrackProcessor::Track track);

    void applyPlayerUpdate(QPointF posOffset, qreal angle, Vector vel, Vector acc);
    StrategicView* getView() const;

public Q_SLOTS:
    void toggleZoom();

private:
    StrategicView* mView;
    PlayerSymbolItem* mPlayerSymbol;
    GridLines* mGridLines;
    QVector<AccelerationMarker*> mAccMarkers;
    QVector<VelocityMarker*> mVelMarkers;
    QMap<uint32_t, StrategicSymbol*> mTracks;
};


