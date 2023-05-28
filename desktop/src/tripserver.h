#ifndef TRIPSERVER_H
#define TRIPSERVER_H

#include <QTcpServer>
#include <clientsocket.h>
#include <QTimer>

class TripServer : public QTcpServer
{
    Q_OBJECT

public:
    TripServer(QObject *parent = 0);

    void sendMessage();

signals:
    void incomingText(QString text);
    void connection();

private slots:
    void getIncomingText(QString text);
    void clean();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    qintptr m_socketDescriptor;
    ClientSocket *socket;
//    QVector<ClientSocket*> vector;
//    QTimer timer;
};

#endif // TRIPSERVER_H
