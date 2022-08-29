#include "include/component.h"

Component::Component(ComponentType type, int x, int y, TwoDeg direction)
    : QObject(), mX(x), mY(y), mType(type), mDirection(direction)
{
    qreal scenePosX = ((x+0.5) - (gGridSize*0.5)) * gGridSize * 2.0;
    qreal scenePosY = ((y+0.5) - (gGridSize*0.5)) * gGridSize * 2.0;
    mPoly = QPolygonF() << QPointF(scenePosX - gGridSceneSize, scenePosY - gGridSceneSize)
            << QPointF(scenePosX + gGridSceneSize, scenePosY - gGridSceneSize)
            << QPointF(scenePosX + gGridSceneSize, scenePosY + gGridSceneSize)
            << QPointF(scenePosX - gGridSceneSize, scenePosY + gGridSceneSize);

    switch (type)
    {
        case ComponentType::HeatSink:
            mTemperature = 25;
            mHeatInRatio = 0.05;
            mHeatOutRatio = 0.01;
            mMass = 100;
            mTexturePoly << QPointF(scenePosX - (0.6*gGridSceneSize), scenePosY - (0.6*gGridSceneSize))
                         << QPointF(scenePosX + (0.6*gGridSceneSize), scenePosY - (0.6*gGridSceneSize))
                         << QPointF(scenePosX + (0.6*gGridSceneSize), scenePosY + (0.6*gGridSceneSize))
                         << QPointF(scenePosX - (0.6*gGridSceneSize), scenePosY + (0.6*gGridSceneSize));
            break;
        case ComponentType::RotateThruster:
        case ComponentType::CruiseThruster:
        case ComponentType::RADAR:
            mTemperature = 25;
            mHeatInRatio = 0.025;
            mHeatOutRatio = 0.025;
            mMass = 100;
            break;
        case ComponentType::Reactor:
            mTemperature = 300;
            mHeatInRatio = 0.025;
            mHeatOutRatio = 0.025;
            mMass = 100;
            mTexturePoly << QPointF(scenePosX, scenePosY - (0.6*gGridSceneSize))
                         << QPointF(scenePosX + (0.6*gGridSceneSize), scenePosY + (0.6*gGridSceneSize))
                         << QPointF(scenePosX - (0.6*gGridSceneSize), scenePosY + (0.6*gGridSceneSize));
    }
}

void Component::applyTemperatureDelta(qreal deltaTemp)
{
    mTemperature += deltaTemp;
    if (mTemperature < 0) mTemperature = 0;
}