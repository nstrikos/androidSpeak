#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include "hotKeyThread.h"
#include "shortcutWidget.h"
#include "optionsDialog.h"
#include "fontsettingsdialog.h"
#include <QClipboard>
#include <QLocalServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void init();
    void shortcutActivated(QString text);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void clientConnected();
    void clientDisconnected();
    void receiveShortCut(QString text);
    void clipboardEnabled();
    void speakClipboard(QClipboard::Mode mode);
    void receiveText(QString from, QString text);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

signals:
    void sendText(QString text);

private slots:
    void historyEditChanged();
    void talkButtonClicked();
    void stopButtonClicked();
    void clearButtonClicked();
    void textEditChanged();
    void showWindow();
    void listWidgetClicked();

    void showShortcutDialog();
    void showOptionsDialog();
    void updateKeys(QVector<HotKey*>hotkeys);
    void showFontSettingsDialog();
    void stopPressed();
    void activatePressed();

    void handleServerConnection();
    void readServerMessage();
    void startServer();
    void disconnectServer();

private:
    Ui::MainWindow *ui;
    bool connected;
    QSystemTrayIcon *trayIcon;
    QAction *quitAction;
    QMenu *trayIconMenu;

    void activate();
    void checkButton();
    void enableControls();
    void disableControls();

    HotKeyThread *hotKeyThread;
    ShortcutWidget *shortcutWindow;

    QMenu* fileMenu;

    QAction* showShortcutAction;
    QAction* optionsDialogAction;

    void createMenu();

    QStringList phrases;
    QStringList codes;
    QStringList ctrls;
    QStringList alts;

    void readSettings();
    void writeSettings();

    QVector<HotKey>hotKeys;

    OptionsDialog *optionsDialog;

    bool m_closeOnSystemTray;
    bool m_startMinimized;

    FontSettingsDialog *fontSettingsDialog;
    QFont appFont;
    QString appFontFamily;
    int appFontSize;
    bool bold;

    QAction *showFontSettingsDialogAction;

    QClipboard *clipboard;

    bool m_useClipboard;
    QString m_clipboardKey;
    bool m_clipboardCtrl;
    bool m_clipboardAlt;
    QString m_stopKey;
    bool m_stopCtrl;
    bool m_stopAlt;
    QString m_activateKey;
    bool m_activateCtrl;
    bool m_activateAlt;

    void setKeys();

    bool checkKeys();
    QString doubleKeys;

    QLocalServer *server;
    QLocalSocket *clientConnection;
};

#endif // MAINWINDOW_H
