#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <QFontDatabase>
#include <QFile>
#include <QLocalSocket>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connected = false;
    disableControls();

    ui->clearButton->setEnabled(false);

    ui->textEdit->installEventFilter(this);

    connect(ui->historyEdit, &QTextEdit::textChanged, this, &MainWindow::historyEditChanged);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::textEditChanged);
    connect(ui->talkButton, &QPushButton::clicked, this, &MainWindow::talkButtonClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearButtonClicked);

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/resources/audio.png");
    trayIcon->setIcon(icon);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::showWindow);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    quitAction->setShortcut(tr("Ctrl+Q"));

    showAction = new QAction(tr("&Show"), this);
    connect(showAction, &QAction::triggered, this, &MainWindow::showWindow);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    hotKeyThread = new HotKeyThread();
    connect(hotKeyThread, &HotKeyThread::sendText, this, &MainWindow::receiveShortCut);
    connect(hotKeyThread, &HotKeyThread::clipboardEnabled, this, &MainWindow::clipboardEnabled);
    connect(hotKeyThread, &HotKeyThread::stopPressed, this, &MainWindow::stopPressed);
    connect(hotKeyThread, &HotKeyThread::activatePressed, this, &MainWindow::activatePressed);

    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::listWidgetClicked);

    //hotKeyThread->start();


    Qt::WindowFlags flags;
    flags = Qt::Dialog;
    flags |= Qt::WindowTitleHint;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::CustomizeWindowHint;
    shortcutWindow = new ShortcutWidget();
    shortcutWindow->setWindowFlags(flags);
    connect(shortcutWindow, &ShortcutWidget::updateKeys, this, &MainWindow::updateKeys);


    createMenu();


    readSettings();
    shortcutWindow->setKeys(&hotKeys);

    optionsDialog = nullptr;
    fontSettingsDialog = nullptr;

    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::changed, this, &MainWindow::speakClipboard);

    server = new QLocalServer(this);
    connect(server, &QLocalServer::newConnection, this, &MainWindow::handleServerConnection);
    startServer();
    clientConnection = nullptr;

    screenReaderDialog = nullptr;
}

void MainWindow::init()
{
    if (!m_startMinimized) {
        show();
        qDebug() << "show";
    }
    else {
        //showMinimized();
        hide();
    }
}

void MainWindow::shortcutActivated(QString text)
{
    ui->textEdit->setText(text);
    activate();
}

MainWindow::~MainWindow()
{
    delete server;

    sendOkScreenReaderClient();

    if (clientConnection != nullptr)
        clientConnection->deleteLater();

    writeSettings();

    if (shortcutWindow != nullptr)
        delete shortcutWindow;

    if (optionsDialog != nullptr)
        delete optionsDialog;

    if (fontSettingsDialog != nullptr)
        delete fontSettingsDialog;

    if (screenReaderDialog != nullptr)
        delete screenReaderDialog;

    delete optionsDialogAction;
    delete showShortcutAction;
    delete showFontSettingsDialogAction;

    hotKeyThread->terminate();
    hotKeyThread->wait();
    delete hotKeyThread;
    delete showAction;
    delete quitAction;
    delete setupOrcaAction;
    delete restoreOrcaAction;
    delete trayIconMenu;
    delete incRateAction;
    delete decRateAction;
    delete incPitchAction;
    delete decPitchAction;
    delete trayIcon;
    delete fileMenu;
    delete optionsMenu;
    delete speechMenu;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_closeOnSystemTray) {
        if (!event->spontaneous() || !isVisible())
            return;
        if (trayIcon->isVisible()) {
            hide();
            event->ignore();
        }
    }
}

void MainWindow::clientConnected()
{
    connected = true;
    emit sendText("command-rate:" + QString::number(m_rate));
    emit sendText("command-pitch:" + QString::number(m_pitch));
    enableControls();

    if (m_orcaSettingsInstalled) {
        m_orcaSettingsInstalled = false;
        writeSettings();
        showOrcaConfirmDialog();
    }
}

void MainWindow::clientDisconnected()
{
    connected = false;
    disableControls();
}

void MainWindow::receiveShortCut(QString text)
{
    if (!connected)
        return;
    ui->textEdit->setText(text);
    activate();
}

void MainWindow::clipboardEnabled()
{
    QString text = clipboard->text(QClipboard::Selection);


    if (!connected)
        return;

    ui->textEdit->setText(text);
    activate();
}

void MainWindow::speakClipboard(QClipboard::Mode mode)
{
    if (mode != QClipboard::Clipboard)
        return;

    QString text = clipboard->text(QClipboard::Clipboard);

    if (!connected || !m_useClipboard)
        return;

    ui->textEdit->setText(text);
    activate();
}

void MainWindow::receiveText(QString from, QString text)
{
    Q_UNUSED(from);

    if (text.startsWith("command-finished:")) {
        QString cmd = "command-finished:";
        int length = cmd.length();
        QString receivedText = text.right(text.length() - length);

        if (receivedText != currentText)
            return;

        sendOkScreenReaderClient();

    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return) {
            // Special tab handling
            activate();
            return true;
        } else {
            return QMainWindow::eventFilter(object, event);
        }
    } else {
        return QMainWindow::eventFilter(object, event);
    }
}

void MainWindow::historyEditChanged()
{
    if (ui->historyEdit->document()->toPlainText() != "")
        ui->clearButton->setEnabled(true);
}

void MainWindow::talkButtonClicked()
{
    activate();
}

void MainWindow::stopButtonClicked()
{
    emit sendText("command-stop");
}

void MainWindow::clearButtonClicked()
{
    ui->historyEdit->clear();
    ui->clearButton->setEnabled(false);
}

void MainWindow::textEditChanged()
{
    checkButton();
}

void MainWindow::showWindow()
{
    this->show();
}

void MainWindow::listWidgetClicked()
{
    if (!connected)
        return;

    QString text = ui->listWidget->currentItem()->text();
    ui->textEdit->setText(text);
    activate();}

void MainWindow::showShortcutDialog()
{
    shortcutWindow->show();
}

void MainWindow::showOptionsDialog()
{
    if (optionsDialog == nullptr)
        optionsDialog = new OptionsDialog(this);

    optionsDialog->setCloseOnSystemTray(m_closeOnSystemTray);
    optionsDialog->setStartMinimized(m_startMinimized);
    optionsDialog->setUseClipboard(m_useClipboard);
    optionsDialog->setSpeak(m_clipboardKey);
    optionsDialog->setSpeakCtrl(m_clipboardCtrl);
    optionsDialog->setSpeakAlt(m_clipboardAlt);
    optionsDialog->setStop(m_stopKey);
    optionsDialog->setStopCtrl(m_stopCtrl);
    optionsDialog->setStopAlt(m_stopAlt);
    optionsDialog->setActivate(m_activateKey);
    optionsDialog->setActivateCtrl(m_activateCtrl);
    optionsDialog->setActivateAlt(m_activateAlt);

    if (optionsDialog->exec()) {
        m_closeOnSystemTray = optionsDialog->closeOnSystemTray();
        m_startMinimized = optionsDialog->startMinimized();
        m_useClipboard = optionsDialog->useClipboard();

        m_clipboardKey = optionsDialog->speak();
        m_clipboardCtrl = optionsDialog->speakCtrl();
        m_clipboardAlt = optionsDialog->speakAlt();

        m_stopKey = optionsDialog->stop();
        m_stopCtrl = optionsDialog->stopCtrl();
        m_stopAlt = optionsDialog->stopAlt();

        m_activateKey = optionsDialog->activate();
        m_activateCtrl = optionsDialog->activateCtrl();
        m_activateAlt = optionsDialog->activateAlt();

        writeSettings();

        setKeys();
    }
}

void MainWindow::updateKeys(QVector<HotKey *>hotkeys)
{
    ui->listWidget->clear();

    HotKey tempKey;
    hotKeys.clear();

    for (int i = 0; i < hotkeys.size(); i++) {
        tempKey.setCode(hotkeys.at(i)->code);
        tempKey.setCtrl(hotkeys.at(i)->ctrl);
        tempKey.setAlt(hotkeys.at(i)->alt);
        tempKey.phrase = hotkeys.at(i)->phrase;
        hotKeys.append(tempKey);
        ui->listWidget->addItem(tempKey.phrase);
    }

    setKeys();
}

void MainWindow::checkButton()
{
    if (!connected)
        return;

    if (ui->textEdit->document()->toPlainText() != "")
        ui->talkButton->setEnabled(true);
}

void MainWindow::enableControls()
{
    statusBar()->showMessage("Connected");
    ui->textEdit->setEnabled(true);
    ui->textEdit->setFocus();
}

void MainWindow::disableControls()
{
    ui->textEdit->setEnabled(false);
    ui->talkButton->setEnabled(false);
    statusBar()->showMessage("Not connected");
}

void MainWindow::createMenu()
{
    showShortcutAction = new QAction(tr("Edit shortcuts..."), this);
    optionsDialogAction = new QAction(tr("Application options..."), this);
    showFontSettingsDialogAction = new QAction(tr("Font settings..."), this);
    showFontSettingsDialogAction->setShortcut(tr("Ctrl+F"));
    incRateAction = new QAction(tr("Increase rate"), this);
    incRateAction->setShortcut(tr("F4"));
    decRateAction = new QAction(tr("Decrease rate"), this);
    decRateAction->setShortcut(tr("F3"));
    incPitchAction = new QAction(tr("Increase pitch"), this);
    incPitchAction->setShortcut(tr("F8"));
    decPitchAction = new QAction(tr("Decrease pitch"), this);
    decPitchAction->setShortcut(tr("F7"));
    setupOrcaAction = new QAction(tr("Setup orca"), this);
    setupOrcaAction->setShortcut(tr("Ctrl+Shift+O"));
    restoreOrcaAction = new QAction(tr("Restore orca settings"), this);
    restoreOrcaAction->setShortcut(tr("Ctrl+Shift+R"));

    connect(showShortcutAction, &QAction::triggered, this, &MainWindow::showShortcutDialog);
    connect(optionsDialogAction, &QAction::triggered, this, &MainWindow::showOptionsDialog);
    connect(showFontSettingsDialogAction, SIGNAL(triggered()), this, SLOT(showFontSettingsDialog()));
    connect(incRateAction, SIGNAL(triggered()), this, SLOT(incRate()));
    connect(decRateAction, SIGNAL(triggered()), this, SLOT(decRate()));
    connect(incPitchAction, SIGNAL(triggered()), this, SLOT(incPitch()));
    connect(decPitchAction, SIGNAL(triggered()), this, SLOT(decPitch()));
    connect(setupOrcaAction, &QAction::triggered, this, &MainWindow::orcaSetupPressed);
    connect(restoreOrcaAction, &QAction::triggered, this, &MainWindow::orcaRestorePressed);

    fileMenu = menuBar()->addMenu(tr("&File"));
    optionsMenu = menuBar()->addMenu(tr("Options"));
    speechMenu = menuBar()->addMenu(tr("Speech"));

    fileMenu->addAction(setupOrcaAction);
    fileMenu->addAction(restoreOrcaAction);
    fileMenu->addAction(quitAction);

    optionsMenu->addAction(showShortcutAction);
    optionsMenu->addAction(optionsDialogAction);
    optionsMenu->addAction(showFontSettingsDialogAction);
    speechMenu->addAction(decRateAction);
    speechMenu->addAction(incRateAction);
    speechMenu->addAction(decPitchAction);
    speechMenu->addAction(incPitchAction);
}

void MainWindow::readSettings()
{
    hotKeys.clear();
    QSettings settings("androidSpeak", "androidSpeak");
    phrases = settings.value("phrases").toStringList();
    codes = settings.value("codes").toStringList();
    ctrls = settings.value("ctrls").toStringList();
    alts = settings.value("alts").toStringList();
    m_closeOnSystemTray = settings.value("systemTray", false).toBool();
    m_startMinimized = settings.value("minimized", false).toBool();
    m_useClipboard = settings.value("useClipboard", false).toBool();
    m_rate = settings.value("rate", 0.0).toDouble();
    m_pitch = settings.value("pitch", 0.0).toDouble();
    m_orcaSettingsInstalled = settings.value("orcaSettingsInstalled", false).toBool();

    appFontFamily = settings.value("fontFamily").toString();
    appFontSize = settings.value("fontSize", 12).toInt();
    bold = settings.value("bold", false).toBool();

    m_clipboardKey = settings.value("clipboardKey", "").toString();
    m_clipboardCtrl = settings.value("clipboardCtrl", false).toBool();
    m_clipboardAlt = settings.value("clipboardAlt", false).toBool();

    m_stopKey = settings.value("stopKey", "").toString();
    m_stopCtrl = settings.value("stopCtrl", false).toBool();
    m_stopAlt = settings.value("stopAlt", false).toBool();

    m_activateKey = settings.value("activateKey", "").toString();
    m_activateCtrl = settings.value("activateCtrl", false).toBool();
    m_activateAlt = settings.value("activateAlt", false).toBool();

    int x = settings.value("x", 0).toInt();
    int y = settings.value("y", 0).toInt();
    int width = settings.value("width", 800).toInt();
    int height = settings.value("height", 600).toInt();
    int size1_1 = settings.value("splitter1-size1", 500).toInt();
    int size1_2 = settings.value("splitter1-size2", 100).toInt();
    int size2_1 = settings.value("splitter2-size1", 100).toInt();
    int size2_2 = settings.value("splitter2-size2", 700).toInt();


    this->setGeometry(x, y, width, height);

    QList<int> sizes;
    sizes << size1_1 << size1_2;
    ui->splitter->setSizes(sizes);

    QList<int> sizes2;
    sizes2 << size2_1 << size2_2;
    ui->splitter_2->setSizes(sizes2);

    appFont.setFamily(appFontFamily);
    appFont.setPointSize(appFontSize);
    appFont.setBold(bold);

    if (appFontFamily == "") {
        if (QFile::exists("/usr/share/androidSpeak/LiberationSans-Regular.ttf")) {
            int id = QFontDatabase::addApplicationFont("/usr/share/androidSpeak/LiberationSans-Regular.ttf");
            QString family = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont monospace(family);
            monospace.setPointSize(12);
            QApplication::setFont(monospace);
        }
    } else {
        QApplication::setFont(appFont);
    }

    HotKey tempKey;

    int size = phrases.size();

    if ( phrases.size() != codes.size())
        return;

    for (int i = 0; i < size; i++) {
        tempKey.phrase = phrases.at(i);
        tempKey.setCode(codes.at(i));
        if (ctrls.at(i) == "true")
            tempKey.setCtrl(true);
        else
            tempKey.setCtrl(false);
        if (alts.at(i) == "true")
            tempKey.setAlt(true);
        else
            tempKey.setAlt(false);
        hotKeys.append(tempKey);
    }

    setKeys();

    ui->listWidget->addItems(phrases);
}

void MainWindow::writeSettings()
{
    phrases.clear();
    codes.clear();
    ctrls.clear();
    alts.clear();

    for (int i = 0; i < hotKeys.size(); i++) {
        phrases.append(hotKeys.at(i).phrase);
        codes.append(hotKeys.at(i).code);
        if (hotKeys.at(i).ctrl == 0)
            ctrls.append("false");
        else
            ctrls.append("true");
        if (hotKeys.at(i).alt == 0)
            alts.append("false");
        else
            alts.append("true");
    }

    QSettings settings("androidSpeak", "androidSpeak");

    settings.setValue("phrases", phrases);
    settings.setValue("codes", codes);
    settings.setValue("ctrls", ctrls);
    settings.setValue("alts", alts);
    settings.setValue("width", this->width());
    settings.setValue("height", this->height());
    settings.setValue("x", this->x());
    settings.setValue("y", this->y());
    settings.setValue("splitter1-size1", ui->splitter->sizes().at(0));
    settings.setValue("splitter1-size2", ui->splitter->sizes().at(1));
    settings.setValue("splitter2-size1", ui->splitter_2->sizes().at(0));
    settings.setValue("splitter2-size2", ui->splitter_2->sizes().at(1));
    settings.setValue("systemTray", m_closeOnSystemTray);
    settings.setValue("minimized", m_startMinimized);
    settings.setValue("useClipboard", m_useClipboard);
    settings.setValue("fontFamily", appFont.family());
    settings.setValue("fontSize", appFont.pointSize());
    settings.setValue("bold", bold);
    settings.setValue("clipboardKey", m_clipboardKey);
    settings.setValue("clipboardCtrl", m_clipboardCtrl);
    settings.setValue("clipboardAlt", m_clipboardAlt);
    settings.setValue("stopKey", m_stopKey);
    settings.setValue("stopCtrl", m_stopCtrl);
    settings.setValue("stopAlt", m_stopAlt);
    settings.setValue("activateKey", m_activateKey);
    settings.setValue("activateCtrl", m_activateCtrl);
    settings.setValue("activateAlt", m_activateAlt);
    settings.setValue("rate", m_rate);
    settings.setValue("pitch", m_pitch);
    settings.setValue("orcaSettingsInstalled", m_orcaSettingsInstalled);
}

void MainWindow::setKeys()
{
    if (!checkKeys()) {
        QMessageBox::information(this, tr("Androidspeak"), doubleKeys);
    }

    HotKey tempKey;

    if (hotKeyThread != nullptr) {
        hotKeyThread->setStopped(true);
    }

    hotKeyThread->wait();

    hotKeyThread->setKeys(hotKeys);

    //    if (m_clipboardKey != "") {
    tempKey.setCode(m_clipboardKey);
    tempKey.setCtrl(m_clipboardCtrl);
    tempKey.setAlt(m_clipboardAlt);
    hotKeyThread->setClipboardKey(tempKey);
    //    }

    //    if (m_stopKey != "") {
    tempKey.setCode(m_stopKey);
    tempKey.setCtrl(m_stopCtrl);
    tempKey.setAlt(m_stopAlt);
    hotKeyThread->setStopKey(tempKey);
    //    }

    //    if (m_activateKey != "") {
    tempKey.setCode(m_activateKey);
    tempKey.setCtrl(m_activateCtrl);
    tempKey.setAlt(m_activateAlt);
    hotKeyThread->setActivateKey(tempKey);
    //    }
    hotKeyThread->start();
}

bool MainWindow::checkKeys()
{
    doubleKeys.clear();

    QVector<HotKey> keys;

    HotKey tmpKey;

    for (int i = 0; i < hotKeys.size(); i++) {
        tmpKey.code = hotKeys.at(i).code;
        tmpKey.ctrl = hotKeys.at(i).ctrl;
        tmpKey.alt = hotKeys.at(i).alt;
        tmpKey.phrase = hotKeys.at(i).phrase;
        keys.append(tmpKey);
    }

    if (m_clipboardKey != "") {
        tmpKey.code = m_clipboardKey;
        tmpKey.ctrl = m_clipboardCtrl;
        tmpKey.alt = m_clipboardAlt;
        tmpKey.phrase = "highlighted text shortcut";
        keys.append(tmpKey);
    }

    if (m_stopKey != "") {
        tmpKey.code = m_stopKey;
        tmpKey.ctrl = m_stopCtrl;
        tmpKey.alt = m_stopAlt;
        tmpKey.phrase = "stop speaking shortcut";
        keys.append(tmpKey);
    }

    if (m_activateKey != "") {
        tmpKey.code = m_activateKey;
        tmpKey.ctrl = m_activateCtrl;
        tmpKey.alt = m_activateAlt;
        tmpKey.phrase = "activate window shortcut";
        keys.append(tmpKey);
    }

    for (int i = 0; i < keys.size(); i++) {
        QString key1 = keys.at(i).code;
        bool ctrl1 = keys.at(i).ctrl;
        bool alt1 = keys.at(i).alt;
        QString key2;
        bool ctrl2;
        bool alt2;
        for (int n = i + 1; n < keys.size(); n++) {
            key2 = keys.at(n).code;
            ctrl2 = keys.at(n).ctrl;
            alt2 = keys.at(n).alt;
            if (key1 == key2) {
                if (ctrl1 == ctrl2) {
                    if (alt1 == alt2) {
                        QString ctrl = ctrl1 ? "true" : "false";
                        QString alt = alt1 ? "true" : "false";
                        doubleKeys += "Key " + key1 + " ctrl:" + ctrl + " alt:" + alt + " is assigned to phrases:\n" +
                                keys.at(i).phrase + " and\n" + keys.at(n).phrase + "\n";
                    }
                }
            }
        }
    }

    if (doubleKeys.isEmpty())
        return true;
    else
        return false;
}

void MainWindow::sendOkScreenReaderClient()
{
    if (clientConnection != nullptr) {
        qDebug() << "Send ok command";
        clientConnection->write("ok");
        clientConnection->flush();
        clientConnection->deleteLater();
        clientConnection = nullptr;
    }
}

void MainWindow::incRate()
{
    if (connected) {
        m_rate += 0.1;
        if (m_rate > 1.0)
            m_rate = 1.0;
        qDebug() << "Rate is: " << m_rate;
        emit sendText("command-rate:" + QString::number(m_rate));
    }
}

void MainWindow::decRate()
{
    if (connected) {
        m_rate -= 0.1;
        if (m_rate < -1.0)
            m_rate = -1.0;
        qDebug() << "Rate is: " << m_rate;
        emit sendText("command-rate:" + QString::number(m_rate));
    }
}

void MainWindow::incPitch()
{
    if (connected) {
        m_pitch += 0.1;
        if (m_pitch > 1.0)
            m_pitch = 1.0;
        qDebug() << "Pitch is: " << m_pitch;
        emit sendText("command-pitch:" + QString::number(m_pitch));
    }
}

void MainWindow::decPitch()
{
    if (connected) {
        m_pitch -= 0.1;
        if (m_pitch < -1.0)
            m_pitch = -1.0;
        qDebug() << "Pitch is: " << m_pitch;
        emit sendText("command-pitch:" + QString::number(m_pitch));
    }
}

void MainWindow::orcaSetupPressed()
{
    if (screenReaderDialog == nullptr) {
        QString text = "This will configure your orca screen reader and your speech dispatcher settings. After this the system will reboot.\nPress Y to continue or press N to return.";
        QString audioFile = "qrc:/resources/screenReaderSetup.wav";
        screenReaderDialog = new ScreenReaderDialog(text, audioFile, this);
        connect(screenReaderDialog, &ScreenReaderDialog::yes, this, &MainWindow::setupScreenReader);
        connect(screenReaderDialog, &ScreenReaderDialog::no, this, &MainWindow::closeScreenReaderDialog);
    }

    screenReaderDialog->start();
}

void MainWindow::orcaRestorePressed()
{
    if (screenReaderDialog == nullptr) {
        QString text = "This will restore your screen reader settings and then will reboot the system.\nPress Y to continue or press N to return.";
        QString audioFile = "qrc:/resources/screenReaderRemoveConfirmation.wav";
        screenReaderDialog = new ScreenReaderDialog(text, audioFile, this);
        connect(screenReaderDialog, &ScreenReaderDialog::yes, this, &MainWindow::restoreScreenReader);
        connect(screenReaderDialog, &ScreenReaderDialog::no, this, &MainWindow::closeScreenReaderDialog);
    }

    screenReaderDialog->start();
}

void MainWindow::showOrcaConfirmDialog()
{
    if (screenReaderDialog == nullptr) {
        QString text = "You updated your screen reader settings. Use tab to check your settings. If you want to keep your current settings press Y. If you want to reboot with your previous settings press N.";
        QString audioFile = "qrc:/resources/screenReaderConfirmation.wav";
        screenReaderDialog = new ScreenReaderDialog(text, audioFile, this);
        connect(screenReaderDialog, &ScreenReaderDialog::yes, this, &MainWindow::closeScreenReaderDialog);
        connect(screenReaderDialog, &ScreenReaderDialog::no, this, &MainWindow::restoreScreenReader);
    }

    screenReaderDialog->start();
}

void MainWindow::setupScreenReader()
{
    setupOrca();
    reboot();
}

void MainWindow::restoreScreenReader()
{
    restoreOrca();
    reboot();
}

void MainWindow::closeScreenReaderDialog()
{
    if (screenReaderDialog == nullptr)
        return;

    screenReaderDialog->stop();
    screenReaderDialog->hide();
    delete screenReaderDialog;
    screenReaderDialog = nullptr;
}

void MainWindow::reboot()
{
    QProcess process;
    QString program;
    if (QFile::exists("/usr/bin/reboot"))
        program = "/usr/bin/reboot";
    else if (QFile::exists("/usr/sbin/reboot"))
        program = "/usr/sbin/reboot";
    else
        program = "reboot";
    process.startDetached(program, QStringList() << "");
}



void MainWindow::showFontSettingsDialog()
{
    if (fontSettingsDialog == NULL)
    {
        fontSettingsDialog = new FontSettingsDialog();
    }
    fontSettingsDialog->setModal(true);
    fontSettingsDialog->setFont(appFont);
    fontSettingsDialog->setPointSize(appFontSize);
    fontSettingsDialog->setBold(bold);

    if (fontSettingsDialog->exec())
    {
        appFont = fontSettingsDialog->getFont();
        appFontSize = fontSettingsDialog->getPointSize();
        bold = fontSettingsDialog->getBold();
        appFont.setPointSize(appFontSize);
        appFont.setBold(bold);
        QApplication::setFont(appFont);
    }
}

void MainWindow::stopPressed()
{
    stopButtonClicked();
}

void MainWindow::activatePressed()
{
    this->show();
    QWidget::activateWindow();
    ui->textEdit->setFocus();
    //Set focus stealing prevention to none
    //If you want to receive focus
    this->setWindowState(Qt::WindowState::WindowActive);
}

void MainWindow::startServer()
{
    if (!server->listen("androidSpeak")) {
        qDebug() << "Unable to start server:" << server->errorString();
        return;
    }

    qDebug() << "Server started. Waiting for incoming connections...";
}

void MainWindow::disconnectServer()
{
    qDebug() << "Server disconnected";

    emit sendText("command-stop");

    if (clientConnection != nullptr)
        clientConnection->deleteLater();
    clientConnection = nullptr;
    //qDebug() << "Got here";
}

void MainWindow::setupOrca()
{
    orcaSetup = new OrcaSetup();
    orcaSetup->start();
    m_closeOnSystemTray = true;
    m_startMinimized = true;
    m_orcaSettingsInstalled = true;
    writeSettings();
    delete orcaSetup;
}

void MainWindow::restoreOrca()
{
    orcaSetup = new OrcaSetup();
    orcaSetup->restore();
    writeSettings();
    delete orcaSetup;
}

void MainWindow::quit()
{
    QApplication::quit();
}

void MainWindow::handleServerConnection()
{
    clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QLocalSocket::readyRead, this, &MainWindow::readServerMessage);
    //connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
    connect(clientConnection, &QLocalSocket::disconnected, this, &MainWindow::disconnectServer);

    qDebug() << "New client connected";
}

void MainWindow::readServerMessage()
{
    QLocalSocket *clientConnection = qobject_cast<QLocalSocket*>(sender());
    if (!clientConnection)
        return;

    QByteArray message = clientConnection->readAll();
    qDebug() << "Received message:" << message;

    // Process the message as needed

    // Reply to the client (optional)
    //clientConnection->write("Message received");
    //clientConnection->flush();

    if (message == "showWindow")
        activatePressed();

    if (message.startsWith("text:")) {
        if (connected) {
            message = message.right(message.length() - 5);
            ui->textEdit->setText(message);
            activate();
        } else {



            qDebug() << "Not connected - send command to client";
            if (clientConnection != nullptr) {
                qDebug() << "clientConnection is active";
                clientConnection->write("not connected");
                clientConnection->flush();
                //clientConnection->deleteLater();
                //clientConnection = nullptr;
                //qDebug() << "Got here";
            }

        }
    }
}

void MainWindow::activate()
{
    if (!connected)
        return;

    QString text = ui->textEdit->document()->toPlainText();
    QString textToSend = "command-text:" + text;
    currentText = text;
    emit sendText(textToSend);
    ui->textEdit->clear();
    ui->talkButton->setEnabled(false);

    QTextCursor cursor = QTextCursor(ui->historyEdit->document());
    cursor.setPosition(0);
    ui->historyEdit->setTextCursor(cursor);
    ui->historyEdit->insertPlainText(text + "\n");
    ui->textEdit->setFocus();
}
