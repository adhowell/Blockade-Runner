#include "signal_track_processor.h"


/*
 * For handling target-selection and proportional navigation guidance logic
 */
class GuidanceProcessor : public SignalTrackProcessor
{
public:
    GuidanceProcessor(WorldObject* parent, QVector<WorldObject*>* worldObjects)
    : SignalTrackProcessor(parent, worldObjects) {}
    ~GuidanceProcessor() = default;

    /**
     * Commands the parent object to rotate towards the most valid
     * target track. If there is no valid track no rotate command is given.
     */
    void guideToMostValidTarget();

private:
    uint32_t mLastTarget {0};
    Bearing mLastValidBearing {0};
};