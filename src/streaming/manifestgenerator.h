#pragma once

#include <QString>

#include "audio/streaminfo.h"
#include "streaming/hook/streamingdto.h"

namespace mixxx {
namespace kinetic {

class ManifestGenerator {
  public:
    // Generates a manifest file for the given track and stream info.
    // Returns the absolute path to the generated file, or empty string on failure.
    // The file will be created in a standard temporary location.
    static QString generate(
            const TrackMetadata& track,
            const QString& streamUrl,
            const QString& extension,
            int bitrate);

    // Helper that takes a full StreamInfo object if available (future)
    // static QString generate(const TrackMetadata& track, const StreamInfo& streamInfo);
};

} // namespace kinetic
} // namespace mixxx
