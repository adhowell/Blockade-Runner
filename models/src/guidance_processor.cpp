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
    auto targetBearing = Bearing(altAtan2);

    Bearing delta = targetBearing - mLastValidBearing;
    if (mLastTarget != track.uid) {
        delta = 0;
        mLastTarget = track.uid;
    }

    mParent->rotate(targetBearing + delta);
    mLastValidBearing = targetBearing + delta;
}