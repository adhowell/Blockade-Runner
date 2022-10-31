#include "include/signal_track_processor.h"


QVector<SignalTrackProcessor::ProcessedTrack> SignalTrackProcessor::getTracks()
{
    return [this]()
    {
        QVector<SignalTrackProcessor::ProcessedTrack> tracks;
        for (auto track : mProcessedTracks.values()) {
            tracks << track;
        }
        return tracks;
    }();
}

void SignalTrackProcessor::computeTracks()
{
    std::for_each(mWorldObjects->begin(), mWorldObjects->end(),
                  [this](auto obj)
                  {
                      // Sensors ignore menu_items belonging to the same faction
                      if (mParent->mId != obj->mId && mParent->mFaction != obj->mFaction)
                      {
                          computeTrack(obj);
                      }
                  });
}

void SignalTrackProcessor::computeTrack(WorldObject* obj)
{
    qreal sepAngle = qAtan2( obj->mP.y() - mParent->mP.y(), obj->mP.x() - mParent->mP.x()) + 0.5*M_PI;
    qreal offBoreAngle = mParent->mAtan2.getDelta(sepAngle);
    for (const auto& sensor : mParent->mSensors)
    {
        // TODO: More fidelity
        if (sensor->withinFOV(offBoreAngle))
        {
            if (mProcessedTracks.find(obj->mId) == mProcessedTracks.end()) {
                mProcessedTracks[obj->mId] = ProcessedTrack{obj->mId, &mParent->mP};
            }
            mProcessedTracks[obj->mId].insertTrack(Track{obj->mP, 0, gTimeStamp});
            return;
        }
    }
}
