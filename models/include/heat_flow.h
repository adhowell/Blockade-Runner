#include "include/component.h"


/**
 * Model for simulating the flow of heat between components
 */
class HeatFlow
{
public:
    explicit HeatFlow(QMap<QPair<int, int>, std::shared_ptr<Component>>& componentMap) : mComponentMap(componentMap) { ; }

    /**
     * Takes the given coordinate->component map, and applies the
     * per-component temperature differentials based on a simple
     * model of heat flow.
     */
    void compute();

private:

    /**
     * Computes the delta between the src component and the neighbouring
     * component at x,y (if extant).
     * @param src - The component.
     * @param x - The x coordinate to check for a neighbouring component.
     * @param y - The y coordinate to check for a neighbouring component.
     */
    void computeHeatFlow(const std::shared_ptr<Component>& src, int x, int y);

    /**
     * Reduce the temperature of a component by 10% for every unconnected face.
     * @param src - The component.
     * @param x - The x coordinate to check for a neighbouring component.
     * @param y - The y coordinate to check for a neighbouring component.
     */
    void computeHeatExhaust(const std::shared_ptr<Component>& src, int x, int y);

    QMap<QPair<int, int>, std::shared_ptr<Component>>& mComponentMap;
    QMap<std::shared_ptr<Component>, int> mValidNeighbours;
    QMap<std::shared_ptr<Component>, qreal> mTempDeltas;
};