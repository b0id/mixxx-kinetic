#include "preferences/dialog/dlgprefstreaming.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "streaming/ui/logindialog.h"

DlgPrefStreaming::DlgPrefStreaming(QWidget* parent, StreamingService* service, UserSettings* config)
        : DlgPreferencePage(parent),
          m_service(service) {
    Q_UNUSED(config);
    auto* mainLayout = new QVBoxLayout(this);

    auto* group = new QGroupBox(service->serviceName(), this);
    auto* layout = new QVBoxLayout(group);

    m_statusLabel = new QLabel("Status: Unknown", group);
    layout->addWidget(m_statusLabel);

    auto* btnLayout = new QHBoxLayout();
    m_loginButton = new QPushButton("Login", group);
    connect(m_loginButton, &QPushButton::clicked, this, &DlgPrefStreaming::invokeLogin);
    btnLayout->addWidget(m_loginButton);

    m_logoutButton = new QPushButton("Logout", group);
    connect(m_logoutButton, &QPushButton::clicked, this, &DlgPrefStreaming::invokeLogout);
    btnLayout->addWidget(m_logoutButton);

    layout->addLayout(btnLayout);
    mainLayout->addWidget(group);
    mainLayout->addStretch();

    connect(m_service, &StreamingService::authStateChanged, this, &DlgPrefStreaming::onAuthStateChanged);

    // Initial update
    slotUpdate();
}

void DlgPrefStreaming::invokeLogin() {
    mixxx::kinetic::LoginDialog dlg(m_service, this);
    if (dlg.exec() == QDialog::Accepted) {
        slotUpdate();
    }
}

void DlgPrefStreaming::invokeLogout() {
    m_service->logout();
    slotUpdate();
}

void DlgPrefStreaming::onAuthStateChanged(AuthState state) {
    Q_UNUSED(state);
    updateUiState();
}

void DlgPrefStreaming::slotResetToDefaults() {
}

void DlgPrefStreaming::slotUpdate() {
    updateUiState();
}

void DlgPrefStreaming::slotApply() {
}

void DlgPrefStreaming::updateUiState() {
    if (m_service->isAuthenticated()) {
        m_statusLabel->setText("Status: <b>Logged In</b>");
        m_loginButton->setVisible(false);
        m_logoutButton->setVisible(true);
    } else {
        m_statusLabel->setText("Status: Not Logged In");
        m_loginButton->setVisible(true);
        m_logoutButton->setVisible(false);
    }
}

#include "moc_dlgprefstreaming.cpp"
