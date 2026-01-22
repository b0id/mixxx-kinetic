#pragma once

#include "preferences/dialog/dlgpreferencepage.h"
#include "preferences/usersettings.h"
#include "streaming/hook/streamingservice.h"

class QLabel;
class QPushButton;

class DlgPrefStreaming : public DlgPreferencePage {
    Q_OBJECT
  public:
    DlgPrefStreaming(QWidget* parent, StreamingService* service, UserSettings* config);

  public slots:
    void invokeLogin();
    void invokeLogout();
    void slotResetToDefaults() override;
    void slotUpdate() override;
    void slotApply() override;

  private slots:
    void onAuthStateChanged(AuthState state);

  private:
    void updateUiState();

    StreamingService* m_service;

    QLabel* m_statusLabel;
    QPushButton* m_loginButton;
    QPushButton* m_logoutButton;
};
