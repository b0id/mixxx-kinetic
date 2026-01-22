#pragma once

#include <QLineEdit>
#include <QListWidget>
#include <QWidget>

#include "streaming/hook/streamingdto.h"
#include "streaming/hook/streamingservice.h"

namespace mixxx {
namespace kinetic {

class StreamBrowserWidget : public QWidget {
    Q_OBJECT
  public:
    explicit StreamBrowserWidget(StreamingService* service, QWidget* parent = nullptr);
    ~StreamBrowserWidget() override = default;

  signals:
    // Emitted when a track is selected and a manifest is generated.
    // The filePath is the absolute path to the .knt manifest.
    void loadTrackToDeck1(const QString& filePath);
    // Can add deck 2 later

  public slots:
    void search(const QString& query);

  private slots:
    void onSearchReturnPressed();
    void onResults(const QVector<TrackMetadata>& results);
    void onLoadClicked();
    void onItemDoubleClicked(QListWidgetItem* item);

  private:
    StreamingService* m_service; // Owned by caller
    QLineEdit* m_searchBox;
    QListWidget* m_resultsList;

    // Store metadata for current results
    QVector<TrackMetadata> m_currentResults;
};

} // namespace kinetic
} // namespace mixxx
