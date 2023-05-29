#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include "hotKeyThread.h"
#include "shortcutWidget.h"
#include "optionsDialog.h"

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

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

signals:
    void sendText(QString text);

private slots:
    void historyEditChanged();
    void talkButtonClicked();
    void clearButtonClicked();
    void textEditChanged();
    void showWindow();
    void listWidgetClicked();

    void showShortcutDialog();
    void showOptionsDialog();
    void updateKeys(QVector<HotKey*>hotkeys);

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
};

#endif // MAINWINDOW_H
