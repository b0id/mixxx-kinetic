#include "streaming/ui/streambrowserwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "streaming/manifestgenerator.h"
#include "util/logger.h"

namespace mixxx {
namespace kinetic {

namespace {
const Logger kLogger("StreamBrowser");
}

StreamBrowserWidget::StreamBrowserWidget(StreamingService* service, QWidget* parent)
        : QWidget(parent),
          m_service(service) {
    auto* layout = new QVBoxLayout(this);

    // Header
    layout->addWidget(new QLabel("<b>Beatport Streaming</b>", this));

    // Search Box
    m_searchBox = new QLineEdit(this);
    m_searchBox->setPlaceholderText("Search Artist, Title, Local...");
    connect(m_searchBox, &QLineEdit::returnPressed, this, &StreamBrowserWidget::onSearchReturnPressed);
    layout->addWidget(m_searchBox);

    // Results List
    m_resultsList = new QListWidget(this);
    connect(m_resultsList, &QListWidget::itemDoubleClicked, this, &StreamBrowserWidget::onItemDoubleClicked);
    layout->addWidget(m_resultsList);

    // Controls
    auto* btnLoad = new QPushButton("Load to Deck 1", this);
    connect(btnLoad, &QPushButton::clicked, this, &StreamBrowserWidget::onLoadClicked);
    layout->addWidget(btnLoad);

    // Connect Service Signals
    if (m_service) {
        connect(m_service, &StreamingService::searchResultsReceived, this, &StreamBrowserWidget::onResults);
        // Note: Error handling signal should also be connected
    }
}

void StreamBrowserWidget::onSearchReturnPressed() {
    search(m_searchBox->text());
}

void StreamBrowserWidget::search(const QString& query) {
    if (!m_service)
        return;
    m_resultsList->clear();
    m_currentResults.clear();
    m_searchBox->setEnabled(false); // Disable while searching

    SearchQuery q;
    q.text = query;
    q.limit = 50;

    m_service->search(q);
}

void StreamBrowserWidget::onResults(const QVector<TrackMetadata>& results) {
    m_searchBox->setEnabled(true);
    m_currentResults = results;
    m_resultsList->clear();

    for (const auto& track : results) {
        QString label = QString("%1 - %2").arg(track.artist, track.title);
        auto* item = new QListWidgetItem(label);
        // Store index as data to map back to m_currentResults
        item->setData(Qt::UserRole, QVariant::fromValue(track.remoteId));
        m_resultsList->addItem(item);
    }
}

void StreamBrowserWidget::onItemDoubleClicked(QListWidgetItem* item) {
    Q_UNUSED(item);
    onLoadClicked();
}

void StreamBrowserWidget::onLoadClicked() {
    auto items = m_resultsList->selectedItems();
    if (items.isEmpty())
        return;

    int row = m_resultsList->row(items.first());
    if (row < 0 || row >= m_currentResults.size())
        return;

    const TrackMetadata& track = m_currentResults[row];
    kLogger.info() << "Requesting stream info for" << track.remoteId;

    // We need to fetch stream info first.
    connect(m_service, &StreamingService::streamInfoReceived, this, [this, track](const StreamInfo& info) {
        // Verify ID matches
        if (info.trackId != track.remoteId)
            return;

        // Derive extension from mimeType or codec if possible, else "mp3"
        QString extension = "mp3";
        if (info.codec == "aac")
            extension = "aac";

        // Generate Manifest
        QString kntPath = ManifestGenerator::generate(track, info.streamUrl.toString(), extension, info.bitrate);
        if (!kntPath.isEmpty()) {
            emit loadTrackToDeck1(kntPath);
        } else {
            QMessageBox::warning(this, "Error", "Failed to generate manifest");
        }

        // Disconnect purely for this one-shot (fragile but ok for prototype)
        disconnect(m_service, &StreamingService::streamInfoReceived, this, nullptr);
    });

    m_service->getStreamInfo(track.remoteId);
}

} // namespace kinetic
} // namespace mixxx

#include "moc_streambrowserwidget.cpp"
