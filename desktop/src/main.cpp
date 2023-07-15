#include <QApplication>
#include <QSharedMemory>
#include <QSharedMemory>
#include <QSharedMemory>

#include <signal.h>

#include "mainwindow.h"
#include "client.h"
#include "screenReaderClient.h"

static QSharedMemory sharedMemory;

void signalhandler(int sig)
{
    //When the application receives a segmentation fault
    //terminate or kill
    //it will detach the sharedMemory
    //This is needed mainly in ubuntu unity
    //Without this loging out prevents detaching sharedMemory
    printf("\nquit the application by signal(%d).\n", sig);
    sharedMemory.detach();
}

void catchUnixSignals(std::initializer_list<int> quitSignals) {
    auto handler = [](int sig) -> void {
        // blocking and not aysnc-signal-safe func are valid
        printf("\nquit the application by signal(%d).\n", sig);
        QCoreApplication::quit();
    };

    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}

void sendShowWindow()
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

void startApplication(MainWindow &mainWindow, Client &client)
{
    signal(SIGSEGV, signalhandler);
#ifndef Q_OS_WIN
    signal(SIGKILL, signalhandler);
#endif
    signal(SIGTERM, signalhandler);

    mainWindow.init();


    QObject::connect(&mainWindow, &MainWindow::sendText, &client, &Client::sendMessage);
    QObject::connect(&client, &Client::clientConnected, &mainWindow, &MainWindow::clientConnected);
    QObject::connect(&client, &Client::clientDisconnected, &mainWindow, &MainWindow::clientDisconnected);
    QObject::connect(&client, &Client::newMessage, &mainWindow, &MainWindow::receiveText);
}

int main(int argc, char *argv[])
{
    //Three ways of starting:
    //1. Normal start of application
    //2. If application is running, send a show window message
    //3. If application name is followed by text, send text to the running application
    if (argc == 1) {

        QApplication app(argc, argv);

        sharedMemory.setKey("androidSpeak");

        if (!sharedMemory.create(1))
        {
            sendShowWindow();
            return 1;            
        } else {
            MainWindow mainWindow;
            Client client;
            startApplication(mainWindow, client);
            return app.exec();
        }
    } else {
        QCoreApplication a(argc, argv);

        if (argc < 2)
            QCoreApplication::quit();

        QString inputText;


        for (int i = 1; i < argc; i++) {
            QString arg = QString::fromUtf8(argv[i]);
            arg.replace("\"", "");
            arg.replace("(", "");
            arg.replace(")", "");
            inputText = inputText + arg.toUtf8() + " ";
        }

        catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
        ScreenReaderClient screenReaderClient(inputText);

        return a.exec();
    }
}
