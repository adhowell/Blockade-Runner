#include "include/heat_flow.h"


void HeatFlow::compute()
{
    // Temperature exhaust flow
    QMapIterator compIter(mComponentMap);
    while (compIter.hasNext())
    {
        compIter.next();
        int x = compIter.key().first;
        int y = compIter.key().second;
        computeHeatExhaust(compIter.value(), x-1, y);
        computeHeatExhaust(compIter.value(), x+1, y);
        computeHeatExhaust(compIter.value(), x, y-1);
        computeHeatExhaust(compIter.value(), x, y+1);
    }

    // Temperature flow between components
    compIter.toFront();
    while (compIter.hasNext())
    {
        compIter.next();
        auto c = compIter.value();
        int x = compIter.key().first;
        int y = compIter.key().second;

        computeHeatFlow(c, x-1, y);
        computeHeatFlow(c, x+1, y);
        computeHeatFlow(c, x, y-1);
        computeHeatFlow(c, x, y+1);
    }
    for (auto c : mTempDeltas.keys())
    {
        c->applyTemperatureDelta(mTempDeltas[c]);
    }
}

void HeatFlow::computeHeatFlow(const std::shared_ptr<Component>& src, int x, int y)
{
    if (mComponentMap.contains({x, y}))
    {
        auto dst = mComponentMap[{x, y}];
        if (src->getTemperature() > dst->getTemperature())
        {
            qreal delta = (src->getTemperature()-dst->getTemperature()) * (dst->getHeatInRatio()) / mValidNeighbours[src];
            mTempDeltas[src] -= delta;
            mTempDeltas[dst] += delta;
        }
    }
}

void HeatFlow::computeHeatExhaust(const std::shared_ptr<Component>& src, int x, int y)
{
    if (!mComponentMap.contains({x, y}))
    {
        src->applyTemperatureDelta(-src->getTemperature()*0.001);
    }
    else
    {
        mValidNeighbours[src]++;
    }
}