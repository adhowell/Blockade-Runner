#include "signal_track_processor.h"


/*
 * For handling target-selection and proportional navigation guidance logic
 */
class GuidanceProcessor : public SignalTrackProcessor
{
public:
    explicit GuidanceProcessor(WorldObject* parent) : SignalTrackProcessor(parent) {}
    ~GuidanceProcessor() = default;

    /**
     * Commands the parent object to rotate towards the most valid
     * target track. If there is no valid track no rotate command is given.
     *
     * @param worldObjects - All of the world objects currently in the simulation
     */
    void guideToMostValidTarget(const QVector<WorldObject*>& worldObjects);

private:
    uint32_t mNullTrackCounter {0};
};