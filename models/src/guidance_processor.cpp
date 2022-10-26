#include "include/guidance_processor.h"


void GuidanceProcessor::guideToMostValidTarget(const QVector<WorldObject*>& worldObjects)
{
    for (auto const& obj : worldObjects)
    {
        // Sensors ignore menu_items belonging to the same faction
        if (mParent->mId == obj->mId || mParent->mFaction == obj->mFaction)
            continue;

        auto track = computeTrack(obj);
        if (track.has_value())
        {
            qreal altAtan2 = qAtan2(track->offset.y(), track->offset.x()) + 0.5*M_PI;
            mParent->rotate(Bearing(altAtan2));
        }
    }
}