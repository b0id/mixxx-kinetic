#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>

#include "streaming/hook/streamingservice.h"

namespace mixxx {
namespace kinetic {

class LoginDialog : public QDialog {
    Q_OBJECT
  public:
    explicit LoginDialog(StreamingService* service, QWidget* parent = nullptr);

  private slots:
    void onStartClicked();
    void onCredentialsSubmit();
    void onCancelClicked();

    // Connected to StreamingService/OAuth signals via Service
    // Note: StreamingService needs to expose these, or we connect to OAuthManager?
    // StreamingService usually abstracts this. We might need to add signals to StreamingService.
    void onDeviceCodeReceived(const QString& userCode, const QUrl& verificationUrl);
    void onAuthSuccess();
    void onAuthError(const QString& msg);

  private:
    void setupUi();
    void showPage(int index);
    bool requiresCredentialAuth() const;

    StreamingService* m_service;

    QStackedWidget* m_stack;

    // Page 0: Start (for browser/device flow)
    QWidget* m_pageStart;

    // Page 0b: Credentials (for Beatport session cookie flow)
    QWidget* m_pageCredentials;
    QLineEdit* m_editUsername;
    QLineEdit* m_editPassword;
    QPushButton* m_btnSubmitCredentials;

    // Page 1: Waiting for User (Device Code)
    QWidget* m_pageCode;
    QLabel* m_lblCode;
    QLabel* m_lblUrl;
    QLabel* m_lblInstruction1;
    QLabel* m_lblInstruction2;
    QPushButton* m_btnCopy;

    // Page 2: Success
    QWidget* m_pageSuccess;

    // Page 3: Error
    QWidget* m_pageError;
    QLabel* m_lblError;
};

} // namespace kinetic
} // namespace mixxx
