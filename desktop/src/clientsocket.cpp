#include "clientsocket.h"
#include <QDataStream>
#include <QDate>
#include <QTime>
//#include <QTextCodec>

ClientSocket::ClientSocket(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error2(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    //    connect(this, SIGNAL(disconnected()), this, SLOT(getSocket()));

    nextBlockSize = 0;
}

void ClientSocket::sendMessage()
{
    generateRandomTrip();
}

void ClientSocket::error2(QAbstractSocket::SocketError error)
{
    qDebug() << error;
}

void ClientSocket::readClient()
{
        QDataStream in(this);
        in.setVersion(QDataStream::Qt_4_3);

        if (nextBlockSize == 0) {
            if (bytesAvailable() < sizeof(quint16))
                return;
            in >> nextBlockSize;
        }
        if (bytesAvailable() < nextBlockSize)
            return;

    //    qDebug() << bytesAvailable();


        QString text;

        in >> text;
    //    this->readAll();
        emit getText(text);
    //    generateRandomTrip();

    //    close();
    //    disconnectFromHost();
    //    // get the information
//    QByteArray Data = readAll();
//    QString DataAsString = QString::fromStdString(Data.toStdString());

    // will write on server side window
//    qDebug() << " Data in: " << DataAsString;
//    emit getText(DataAsString);

//    close();
}

void ClientSocket::getSocket()
{
    qDebug() << "disconnect";
    close();
}

void ClientSocket::generateRandomTrip()
{
    //    QByteArray block;
    //    QDataStream out(&block, QIODevice::WriteOnly);
    //    out.setVersion(QDataStream::Qt_4_3);
    //    QString text = "ok";
    //    out << quint16(0) << text;
    //    out.device()->seek(0);
    //    out << quint16(block.size() - sizeof(quint16));

    //    write(block);
    close();
}
