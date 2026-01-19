#pragma once

#include "sources/soundsourcekineticproxy.h"
#include "sources/soundsourceprovider.h"

namespace mixxx {

class SoundSourceKineticProvider : public SoundSourceProvider {
  public:
    QString getDisplayName() const override {
        return "Kinetic Stream";
    }

    QStringList getSupportedFileTypes() const override {
        return {"knt"};
    }

    SoundSourceProviderPriority getPriorityHint(
            const QString& supportedFileType) const override {
        Q_UNUSED(supportedFileType);
        // High priority to ensure we handle our own manifest files
        return SoundSourceProviderPriority::Highest;
    }

    SoundSourcePointer newSoundSource(const QUrl& url) override {
        // We assume the URL points to a local .knt file
        if (!url.isLocalFile()) {
            return nullptr;
        }
        return std::make_shared<SoundSourceKineticProxy>(url.toLocalFile());
    }
};

} // namespace mixxx
