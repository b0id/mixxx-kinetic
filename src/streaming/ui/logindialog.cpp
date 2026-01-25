#include "streaming/ui/logindialog.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "streaming/hook/beatportservice.h"
#include "util/logger.h"

namespace mixxx {
namespace kinetic {

namespace {
const Logger kLogger("LoginDialog");
}

LoginDialog::LoginDialog(StreamingService* service, QWidget* parent)
        : QDialog(parent),
          m_service(service) {
    setWindowTitle(QString("Login to %1").arg(service->serviceName()));
    resize(400, 300);
    setupUi();

    // Connect Service Signals
    // We assume StreamingService exposes these. If not, we'll need to add them.
    // OAuthManager usually emits 'deviceCodeReceived'.
    // We need to bridge this in StreamingService or access OAuthManager directly?
    // Best practice: StreamingService abstracts it.

    // For now, let's assume we can connect to service signals we will add.
    connect(m_service, &StreamingService::authStateChanged, this, [this](AuthState state) {
        if (state == AuthState::LoggedIn) {
            onAuthSuccess();
        } else if (state == AuthState::Error) {
            // onAuthError handled by separate signal usually
        }
    });

    connect(m_service, &StreamingService::loginError, this, &LoginDialog::onAuthError);

    // Connect device code signal
    connect(m_service, &StreamingService::deviceAuthReady, this, &LoginDialog::onDeviceCodeReceived);

    // Show appropriate initial page based on service type
    if (requiresCredentialAuth()) {
        m_stack->setCurrentWidget(m_pageCredentials);
    } else {
        m_stack->setCurrentWidget(m_pageStart);
    }
}

bool LoginDialog::requiresCredentialAuth() const {
    // Beatport requires session cookie flow with username/password
    // Other services (Tidal, SoundCloud) will use browser/device flow
    return m_service->serviceId() == "beatport";
}

void LoginDialog::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    m_stack = new QStackedWidget(this);
    mainLayout->addWidget(m_stack);

    // --- Page 0: Start (for browser/device flow services like Tidal) ---
    m_pageStart = new QWidget(this);
    auto* layoutStart = new QVBoxLayout(m_pageStart);
    layoutStart->addWidget(new QLabel("Click 'Login' to start the authorization process in your browser.", m_pageStart));
    auto* btnStart = new QPushButton("Login", m_pageStart);
    connect(btnStart, &QPushButton::clicked, this, &LoginDialog::onStartClicked);
    layoutStart->addWidget(btnStart);
    m_stack->addWidget(m_pageStart);

    // --- Page 0b: Credentials (for Beatport session cookie flow) ---
    m_pageCredentials = new QWidget(this);
    auto* layoutCreds = new QVBoxLayout(m_pageCredentials);
    layoutCreds->addWidget(new QLabel("Enter your Beatport credentials:", m_pageCredentials));

    layoutCreds->addWidget(new QLabel("Username:", m_pageCredentials));
    m_editUsername = new QLineEdit(m_pageCredentials);
    m_editUsername->setPlaceholderText("your_beatport_username");
    layoutCreds->addWidget(m_editUsername);

    layoutCreds->addWidget(new QLabel("Password:", m_pageCredentials));
    m_editPassword = new QLineEdit(m_pageCredentials);
    m_editPassword->setEchoMode(QLineEdit::Password);
    m_editPassword->setPlaceholderText("Password");
    layoutCreds->addWidget(m_editPassword);

    m_btnSubmitCredentials = new QPushButton("Login", m_pageCredentials);
    connect(m_btnSubmitCredentials, &QPushButton::clicked, this, &LoginDialog::onCredentialsSubmit);
    connect(m_editPassword, &QLineEdit::returnPressed, this, &LoginDialog::onCredentialsSubmit);
    layoutCreds->addWidget(m_btnSubmitCredentials);

    layoutCreds->addStretch();
    m_stack->addWidget(m_pageCredentials);

    // --- Page 1: Code ---
    m_pageCode = new QWidget(this);
    auto* layoutCode = new QVBoxLayout(m_pageCode);
    m_lblInstruction1 = new QLabel("1. Go to this URL:", m_pageCode);
    layoutCode->addWidget(m_lblInstruction1);

    m_lblUrl = new QLabel("<a href='#'>Verification URL</a>", m_pageCode);
    m_lblUrl->setOpenExternalLinks(true);
    m_lblUrl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layoutCode->addWidget(m_lblUrl);

    m_lblInstruction2 = new QLabel("2. Enter this code:", m_pageCode);
    layoutCode->addWidget(m_lblInstruction2);

    m_lblCode = new QLabel("????-????", m_pageCode);
    QFont font = m_lblCode->font();
    font.setPointSize(24);
    font.setBold(true);
    m_lblCode->setFont(font);
    m_lblCode->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layoutCode->addWidget(m_lblCode);

    m_btnCopy = new QPushButton("Copy Code", m_pageCode);
    connect(m_btnCopy, &QPushButton::clicked, this, [this]() {
        QApplication::clipboard()->setText(m_lblCode->text());
    });
    layoutCode->addWidget(m_btnCopy);

    layoutCode->addWidget(new QLabel("Waiting for authorization...", m_pageCode));
    m_stack->addWidget(m_pageCode);

    // --- Page 2: Success ---
    m_pageSuccess = new QWidget(this);
    auto* layoutSuccess = new QVBoxLayout(m_pageSuccess);
    layoutSuccess->addWidget(new QLabel("Login Successful!", m_pageSuccess));
    auto* btnClose = new QPushButton("Close", m_pageSuccess);
    connect(btnClose, &QPushButton::clicked, this, &LoginDialog::accept);
    layoutSuccess->addWidget(btnClose);
    m_stack->addWidget(m_pageSuccess);

    // --- Page 3: Error ---
    m_pageError = new QWidget(this);
    auto* layoutError = new QVBoxLayout(m_pageError);
    m_lblError = new QLabel("Error", m_pageError);
    layoutError->addWidget(m_lblError);
    auto* btnRetry = new QPushButton("Retry", m_pageError);
    connect(btnRetry, &QPushButton::clicked, this, [this]() {
        // Return to appropriate page based on service type
        if (requiresCredentialAuth()) {
            m_stack->setCurrentWidget(m_pageCredentials);
        } else {
            onStartClicked();
        }
    });
    layoutError->addWidget(btnRetry);
    m_stack->addWidget(m_pageError);
}

void LoginDialog::onStartClicked() {
    m_stack->setCurrentWidget(m_pageCode); // Switch to waiting state (optimistic)
    // Clear old labels
    m_lblCode->setText("Loading...");
    m_service->initiateLogin();
}

void LoginDialog::onCredentialsSubmit() {
    QString username = m_editUsername->text().trimmed();
    QString password = m_editPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        onAuthError("Please enter both email and password");
        return;
    }

    // Disable inputs while authenticating
    m_editUsername->setEnabled(false);
    m_editPassword->setEnabled(false);
    m_btnSubmitCredentials->setEnabled(false);
    m_btnSubmitCredentials->setText("Authenticating...");

    // Cast to BeatportService to access authenticate() method
    // This is safe because we only show credentials page for Beatport
    auto* beatportService = qobject_cast<BeatportService*>(m_service);
    if (beatportService) {
        beatportService->authenticate(username, password);
    } else {
        // Fallback - shouldn't happen
        onAuthError("Internal error: Service does not support credential authentication");
    }
}

void LoginDialog::onDeviceCodeReceived(const QString& userCode, const QUrl& verificationUrl) {
    if (userCode.isEmpty()) {
        // Browser Flow
        m_lblInstruction1->setText("Please verify the application in your browser.");
        m_lblUrl->setText(QString("<a href=\"%1\">Click here if browser implies it failed/did not open</a>").arg(verificationUrl.toString()));
        m_lblInstruction2->setVisible(false);
        m_lblCode->setVisible(false);
        m_btnCopy->setVisible(false);

        QDesktopServices::openUrl(verificationUrl);
    } else {
        // Device Flow
        m_lblInstruction1->setText("1. Go to this URL:");
        m_lblUrl->setText(QString("<a href=\"%1\">%1</a>").arg(verificationUrl.toString()));
        m_lblInstruction2->setVisible(true);
        m_lblCode->setVisible(true);
        m_lblCode->setText(userCode);
        m_btnCopy->setVisible(true);
    }
    m_stack->setCurrentWidget(m_pageCode);
}

void LoginDialog::onAuthSuccess() {
    m_stack->setCurrentWidget(m_pageSuccess);
    // Auto-close after delay?
}

void LoginDialog::onAuthError(const QString& msg) {
    m_lblError->setText(QString("Login Failed:\n%1").arg(msg));

    // Re-enable credentials inputs if we were using credential auth
    if (requiresCredentialAuth()) {
        m_editUsername->setEnabled(true);
        m_editPassword->setEnabled(true);
        m_btnSubmitCredentials->setEnabled(true);
        m_btnSubmitCredentials->setText("Login");
    }

    m_stack->setCurrentWidget(m_pageError);
}

void LoginDialog::onCancelClicked() {
    reject();
}

void LoginDialog::showPage(int index) {
    m_stack->setCurrentIndex(index);
}

} // namespace kinetic
} // namespace mixxx

#include "moc_logindialog.cpp"
