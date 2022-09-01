#include "include/signal_track_processor.h"


QVector<SignalTrackProcessor::Track> SignalTrackProcessor::getTracks(const QVector<WorldObject*>& worldObjects)
{
    QVector<SignalTrackProcessor::Track> tracks;
    std::for_each(worldObjects.cbegin(), worldObjects.cend(),
                  [this, &tracks](auto obj)
                  {
                      // Sensors ignore objects belonging to the same faction
                      if (mParent->mId != obj->mId && mParent->mFaction != obj->mFaction)
                      {
                          auto track = computeTrack(obj);
                          if (track.has_value()) tracks << track.value();
                      }
                  });
    return tracks;
}

std::optional<SignalTrackProcessor::Track> SignalTrackProcessor::computeTrack(WorldObject* obj)
{
    qreal offBoreAngle = mParent->mAtan2 - mParent->mP.separationAngle(obj->mP);
    for (const auto& sensor : mParent->mSensors)
    {
        // TODO: More fidelity
        //if (sensor->withinFOV(offBoreAngle))
        //{
        auto t = Track();
        t.bearingOffBore = offBoreAngle;
        t.distance = mParent->mP.getDistance(obj->getPosVector());
        t.factionKnown = true;
        t.receivedPower = 1;
        t.targetUid = obj->getId();
        return t;
        //}
    }
    return {};
}

void SignalTrackProcessor::updateSensors()
{
    mParent->updateSensors();
}