#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QTcpSocket>
#include <QDebug>
#include <QThread>


class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(qintptr ID, QObject *parent = 0);
    void run();


signals:
    void error(QTcpSocket::SocketError socketerror);
    void getText(QString text);

public slots:
    void readyRead();
    void disconnected();
    void close();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    quint16 nextBlockSize;
};

#endif // MYTHREAD_H
