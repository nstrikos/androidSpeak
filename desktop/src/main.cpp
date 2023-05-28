#include <QApplication>

//#include "chatdialog.h"
#include "mainwindow.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //ChatDialog dialog;
    //dialog.show();

    MainWindow mainWindow;
    mainWindow.init();

    Client client;

    QObject::connect(&mainWindow, &MainWindow::sendText, &client, &Client::sendMessage);
    QObject::connect(&client, &Client::clientConnected, &mainWindow, &MainWindow::clientConnected);
    QObject::connect(&client, &Client::clientDisconnected, &mainWindow, &MainWindow::clientDisconnected);

    return app.exec();
}
