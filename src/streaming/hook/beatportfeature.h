#pragma once

#include "library/libraryfeature.h"
#include "preferences/usersettings.h"
#include "streaming/hook/streamingservice.h"
#include "util/parented_ptr.h"

class BeatportFeature : public LibraryFeature {
    Q_OBJECT
  public:
    BeatportFeature(Library* pLibrary, UserSettingsPointer pConfig, std::shared_ptr<StreamingService> pService);
    ~BeatportFeature() override;

    QVariant title() override;

    void bindSidebarWidget(WLibrarySidebar* pSidebarWidget) override;
    TreeItemModel* sidebarModel() const override;

  public slots:
    void activate() override;
    void activateChild(const QModelIndex& index) override;
    void onRightClick(const QPoint& globalPos) override;

  private:
    std::shared_ptr<StreamingService> m_pService;
    parented_ptr<TreeItemModel> m_pSidebarModel;
};
