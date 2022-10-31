#include "include/guidance_processor.h"


void GuidanceProcessor::guideToMostValidTarget()
{
    if (mProcessedTracks.empty()) {
        return;
    }
    // Most valid target is just the most recently seen one
    auto track = *std::max_element(mProcessedTracks.begin(), mProcessedTracks.end(),
                                   [](auto id, auto track)
                                   {
                                       return track.getLastTimestamp();
                                   });
    qreal altAtan2 = qAtan2(track.offset.y(), track.offset.x()) + 0.5*M_PI;
    mParent->rotate(Bearing(altAtan2));
}