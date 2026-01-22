#include "beatportfeature.h"

#include <QIcon>
#include <QStandardItem>

#include "library/library.h"
#include "library/treeitem.h"
#include "library/treeitemmodel.h"

BeatportFeature::BeatportFeature(Library* pLibrary, UserSettingsPointer pConfig, std::shared_ptr<StreamingService> pService)
        : LibraryFeature(pLibrary, pConfig, "ic_library_beatport.svg"),
          m_pService(pService),
          m_pSidebarModel(make_parented<TreeItemModel>(this)) {
    // Create Root Item
    m_pSidebarModel->setRootItem(TreeItem::newRoot(this));
    TreeItem* root = m_pSidebarModel->getRootItem();

    // Add "Search" node
    root->appendChild("Search", "beatport_search");
}

BeatportFeature::~BeatportFeature() {
}

QVariant BeatportFeature::title() {
    return "Beatport";
}

void BeatportFeature::bindSidebarWidget(WLibrarySidebar* pSidebarWidget) {
    // Connect signals if needed
}

TreeItemModel* BeatportFeature::sidebarModel() const {
    return m_pSidebarModel.get();
}

void BeatportFeature::activate() {
    // Switch main view to a search page or something
    // m_pLibrary->switchToView("BeatportView");
}

void BeatportFeature::activateChild(const QModelIndex& index) {
    // Handle click on children
}

void BeatportFeature::onRightClick(const QPoint& globalPos) {
    // Context menu
}

#include "moc_beatportfeature.cpp"
