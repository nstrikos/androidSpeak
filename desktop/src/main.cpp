#include <QApplication>

//#include "chatdialog.h"
#include "mainwindow.h"
#include "client.h"

#include <QSharedMemory>
#include <signal.h>
#include <QLocalSocket>

static QSharedMemory sharedMemory;

void signalhandler(int sig)
{
    //When the application receives a segmentation fault
    //terminate or kill
    //it will detach the sharedMemory
    //This is needed mainly in ubuntu unity
    //Without this loging out prevents detaching sharedMemory
    Q_UNUSED(sig);
    sharedMemory.detach();
}

void sendMessage()
{
    QLocalSocket socket;
    socket.connectToServer("androidSpeak");

    if (!socket.waitForConnected()) {
        qDebug() << "Unable to connect to server:" << socket.errorString();
    }

    QString message = "showWindow";
    qDebug() << "Sending message:" << message;

    socket.write(message.toUtf8());
    socket.flush();

    socket.disconnectFromServer();
    socket.waitForDisconnected();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    sharedMemory.setKey("androidSpeak");
    if(sharedMemory.attach())
    {
        //Do nothing;
        ;
    }

    if (!sharedMemory.create(1))
    {
        sendMessage();
        return 1;
    }

    signal(SIGSEGV, signalhandler);
#ifndef Q_OS_WIN
    signal(SIGKILL, signalhandler);
#endif
    signal(SIGTERM, signalhandler);

    MainWindow mainWindow;
    mainWindow.init();

    Client client;

    QObject::connect(&mainWindow, &MainWindow::sendText, &client, &Client::sendMessage);
    QObject::connect(&client, &Client::clientConnected, &mainWindow, &MainWindow::clientConnected);
    QObject::connect(&client, &Client::clientDisconnected, &mainWindow, &MainWindow::clientDisconnected);

    return app.exec();
}
