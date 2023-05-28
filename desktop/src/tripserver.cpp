#include "tripserver.h"
#include "clientsocket.h"
#include "mythread.h"

TripServer::TripServer(QObject *parent)
    : QTcpServer(parent)
{
//    connect(&timer, SIGNAL(timeout()), this, SLOT(clean()));
//    timer.start(60 * 1000);
}

void TripServer::sendMessage()
{
    emit connection();
}

void TripServer::getIncomingText(QString text)
{
    emit incomingText(text);
}

void TripServer::clean()
{
//    int i = vector.length();
//    for (int i = 0; i < vector.length(); i++){
//        qDebug() << vector.length();
//        ClientSocket *socket = vector.takeFirst();
//        socket->close();
//        delete socket;
//    }
}

void TripServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "Incoming connection";
//    ClientSocket *socket = new ClientSocket(this);
    socket = new ClientSocket(this);
//    vector.append(socket);
    connect(socket, SIGNAL(getText(QString)), this, SLOT(getIncomingText(QString)));
    connect(this, SIGNAL(connection()), socket, SLOT(getSocket()));
    m_socketDescriptor = socketDescriptor;
    socket->setSocketDescriptor(m_socketDescriptor);

//     We have a new connection
//    qDebug() << socketDescriptor << " Connecting...";

//    // Every new connection will be run in a newly created thread
//    MyThread *thread = new MyThread(socketDescriptor, this);

//    // connect signal/slot
//    // once a thread is not needed, it will be beleted later
//    connect(thread, SIGNAL(getText(QString)), this, SLOT(getIncomingText(QString)));
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    connect(this, SIGNAL(connection()), thread, SLOT(close()));

//    thread->start();
}
