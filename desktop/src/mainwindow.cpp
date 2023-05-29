#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

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
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearButtonClicked);

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/resources/audio.png");
    trayIcon->setIcon(icon);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::showWindow);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    hotKeyThread = new HotKeyThread();
    connect(hotKeyThread, &HotKeyThread::sendText, this, &MainWindow::receiveShortCut);

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
}

void MainWindow::init()
{
    if (!m_startMinimized) {
        show();
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
    writeSettings();

    if (shortcutWindow != nullptr)
        delete shortcutWindow;

    if (optionsDialog != nullptr)
        delete optionsDialog;

    delete optionsDialogAction;
    delete showShortcutAction;

    hotKeyThread->terminate();
    hotKeyThread->wait();
    delete hotKeyThread;
    delete quitAction;
    delete trayIconMenu;
    delete trayIcon;
    delete fileMenu;
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
    enableControls();
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

    if (optionsDialog->exec()) {
        m_closeOnSystemTray = optionsDialog->closeOnSystemTray();
        m_startMinimized = optionsDialog->startMinimized();
    }
}

void MainWindow::updateKeys(QVector<HotKey *>hotkeys)
{
    if (hotKeyThread != nullptr) {
        hotKeyThread->setStopped(true);
    }

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

    hotKeyThread->setKeys(hotKeys);
    hotKeyThread->start();
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


    connect(showShortcutAction, &QAction::triggered, this, &MainWindow::showShortcutDialog);
    connect(optionsDialogAction, &QAction::triggered, this, &MainWindow::showOptionsDialog);

    fileMenu = menuBar()->addMenu(tr("Options"));
    fileMenu->addAction(showShortcutAction);
    fileMenu->addAction(optionsDialogAction);
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


    if (hotKeyThread != nullptr) {
        hotKeyThread->setStopped(true);
    }

    hotKeyThread->setKeys(hotKeys);
    hotKeyThread->start();


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
}

void MainWindow::activate()
{
    if (!connected)
        return;

    QString text = ui->textEdit->document()->toPlainText();
    emit sendText(text);    
    ui->textEdit->clear();
    ui->talkButton->setEnabled(false);

    QTextCursor cursor = QTextCursor(ui->historyEdit->document());
    cursor.setPosition(0);
    ui->historyEdit->setTextCursor(cursor);
    ui->historyEdit->insertPlainText(text + "\n");

    ui->textEdit->setFocus();    
}
