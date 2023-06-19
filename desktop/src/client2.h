#ifndef CLIENT2_H
#define CLIENT2_H

#include <QObject>
#include <QLocalSocket>

class QProcess;

class Client2 : public QObject
{
    Q_OBJECT
public:
    explicit Client2(QString data, QObject *parent = nullptr);
    ~Client2();

private slots:
    void connectToServer();
    void sendRequest();
    void readFromServer();
    void connectionClosedByServer();
    void error(QAbstractSocket::SocketError socketError);
    void processFinished();

private:
    void closeConnection();

    QString m_pitch;
    QString m_rate;
    QString m_data;
    QLocalSocket socket;
    quint16 nextBlockSize;
    QProcess *m_process;
};

#endif // CLIENT2_H
