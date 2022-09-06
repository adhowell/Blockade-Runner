#include "include/signal_track_processor.h"


QVector<SignalTrackProcessor::Track> SignalTrackProcessor::getTracks(const QVector<WorldObject*>& worldObjects)
{
    QVector<SignalTrackProcessor::Track> tracks;
    std::for_each(worldObjects.cbegin(), worldObjects.cend(),
                  [this, &tracks](auto obj)
                  {
                      // Sensors ignore menu_items belonging to the same faction
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
    qreal sepAngle = qAtan2( obj->mP.y() - mParent->mP.y(), obj->mP.x() - mParent->mP.x()) + 0.5*M_PI;
    qreal offBoreAngle = mParent->mAtan2.getDelta(sepAngle);
    for (const auto& sensor : mParent->mSensors)
    {
        // TODO: More fidelity
        if (sensor->withinFOV(offBoreAngle))
        {
            return Track{obj->mP - mParent->mP, 0, Faction::Unknown, obj->mId};
        }
    }
    return {};
}

void SignalTrackProcessor::commandRotateToTrack(const QVector<WorldObject*>& worldObjects)
{
    for (auto const& obj : worldObjects)
    {
        // Sensors ignore menu_items belonging to the same faction
        if (mParent->mId == obj->mId || mParent->mFaction == obj->mFaction)
            continue;

        auto track = computeTrack(obj);
        if (track.has_value())
        {
            mParent->rotate(mParent->mAtan2.getDelta(track->offset.getAtan2())*180.0/M_PI);
        }
    }
}

void SignalTrackProcessor::updateSensors()
{
    mParent->updateSensors();
}
