#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>


class ClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    ClientSocket(QObject *parent = 0);

signals:
   void getText(QString text);

private slots:
    void readClient();
    void getSocket();
    void sendMessage();
    void error2(QAbstractSocket::SocketError error);

private:
    void generateRandomTrip();
    quint16 nextBlockSize;
};

#endif // CLIENTSOCKET_H
