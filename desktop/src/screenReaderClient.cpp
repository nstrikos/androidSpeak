#include "screenReaderClient.h"

#include <QHostAddress>
#include <QDataStream>
#include <QCoreApplication>
#include <QProcess>
#include <QFile>

ScreenReaderClient::ScreenReaderClient(QString data, QObject *parent) : QObject(parent)
{
    connect(&socket, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readFromServer()));
    connect(&socket, SIGNAL(errorOccurred(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));

    receivedData = data;
    process = nullptr;
    isRunning = false;
    shouldQuit = false;
    timer.setSingleShot(true);
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &ScreenReaderClient::quit);
    debug = 1;
    if (debug)
        qDebug() << "Constructor called.";

    connectToServer();
}

ScreenReaderClient::~ScreenReaderClient()
{
    if (process != nullptr) {
        process->terminate();
        process->waitForFinished(-1);
        delete process;
    }

    if (debug)
        qDebug() << "Destructor called.";
}

void ScreenReaderClient::connectToServer()
{
    if (debug)
        qDebug() << "Trying to connect to server.";

    socket.connectToServer("androidSpeak");

    nextBlockSize = 0;
}

void ScreenReaderClient::sendRequest()
{
    if (debug)
        qDebug() << "Connected. Sending: " << receivedData;

    QString data = "text:" + receivedData;

    socket.write(data.toUtf8());
    socket.flush();
}

void ScreenReaderClient::readFromServer()
{
    QByteArray message = socket.readAll();

    if (debug)
        qDebug() << "Received message. Message is:" << message;

    if (message == "ok")
        closeConnection();

    if (message == "not connected") {
        startProcess();
    }
}

void ScreenReaderClient::connectionClosedByServer()
{
    if (debug)
        qDebug() << "Connection closed by server.";

    if (!isRunning)
        closeConnection();
}

void ScreenReaderClient::error(QLocalSocket::LocalSocketError socketError)
{
    if (debug)
        qDebug() << "Error occurred:" << socketError;

    if (socketError == QLocalSocket::ServerNotFoundError)
        startProcess();
    else
        closeConnection();
}

void ScreenReaderClient::processFinished()
{
    isRunning = false;
    closeConnection();
}

void ScreenReaderClient::quit()
{
    closeConnection();
}

void ScreenReaderClient::closeConnection()
{
    if (debug) {
        qDebug() << "Closing connection.";
    }
    socket.close();

    QCoreApplication::quit();
}

void ScreenReaderClient::startProcess()
{
    if (debug)
        qDebug() << "Process running is: " << isRunning;

    if (isRunning) {
        if (debug)
            qDebug() << "I will not start another process";
        return;
    }

    if (QFile::exists("/usr/bin/espeak")) {
        if (debug) {
            qDebug() << "Found espeak:" << receivedData;
        }
        if (process == nullptr)
            process = new QProcess;
        connect(process, SIGNAL(finished(int)), this, SLOT(processFinished()));
        QString program = "/usr/bin/espeak";
        if (!isRunning) {
            isRunning = true;
            QStringList list;
            list << receivedData;
            process->start(program, list);
        }
    } else if (QFile::exists("/usr/bin/espeak-ng")) {
        if (debug) {
            qDebug() << "Found espeak-ng:" << receivedData;
        }
        if (process == nullptr)
            process = new QProcess;
        connect(process, SIGNAL(finished(int)), this, SLOT(processFinished()));
        QString program = "espeak-ng";
        if (!isRunning) {
            isRunning = true;
            QStringList list;
            list << receivedData;
            process->start(program, list);
        }
    } else {
        if (debug) {
            qDebug() << "Cannot find espeak or espeak-ng";
        }
        //Start a timer to close application
        //If we wall exit now, application will not exit because it has not reach app.exec() yet
        shouldQuit = true;
        timer.start();
    }
}
