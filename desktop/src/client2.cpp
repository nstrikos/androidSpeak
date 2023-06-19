#include "client2.h"

#include <QHostAddress>
#include <QDataStream>
#include <QCoreApplication>
#include <QProcess>

Client2::Client2(QString data, QObject *parent) : QObject(parent)
{
    connect(&socket, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readFromServer()));
    //connect(&socket, SIGNAL(error(QLocalSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    m_data = "text:" + data;
    m_process = nullptr;
    connectToServer();
}

Client2::~Client2()
{
    if (m_process != nullptr) {
        m_process->terminate();
        m_process->waitForFinished(-1);
        delete m_process;
    }
}

void Client2::connectToServer()
{
    socket.connectToServer("androidSpeak");
    nextBlockSize = 0;
}

void Client2::sendRequest()
{
    socket.write(m_data.toUtf8());
    socket.flush();

    //socket.disconnectFromServer();
    //socket.waitForDisconnected();
}

void Client2::readFromServer()
{
    QByteArray message = socket.readAll();
    qDebug() << "Received message:" << message;

    if (message == "ok")
        QCoreApplication::quit();

}

void Client2::connectionClosedByServer()
{
    closeConnection();
}

void Client2::error(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    m_process = new QProcess;
    connect(m_process, SIGNAL(finished(int)), this, SLOT(processFinished()));

    //    QString lang;
    //    if (m_language == "Greek")
    //        lang = "el";
    //    else
    //        lang = "en";

    QString process = "espeak \"" + m_data + "\"";
    //    QString process = "espeak " + m_pitch;

    m_process->start(process);
    //    closeConnection();
}

void Client2::processFinished()
{
    closeConnection();
}

void Client2::closeConnection()
{
    socket.close();
    QCoreApplication::quit();
}
