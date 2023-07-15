#ifndef SCREENREADERCLIENT_H
#define SCREENREADERCLIENT_H

#include <QObject>
#include <QLocalSocket>
#include <QTimer>

class QProcess;

class ScreenReaderClient : public QObject
{
    Q_OBJECT
public:
    explicit ScreenReaderClient(QString data, QObject *parent = nullptr);
    ~ScreenReaderClient();

private slots:
    void connectToServer();
    void sendRequest();
    void readFromServer();
    void connectionClosedByServer();
    void error(QLocalSocket::LocalSocketError socketError);
    void processFinished();
    void quit();

private:
    void closeConnection();
    void startProcess();

    QString receivedData;
    QLocalSocket socket;
    quint16 nextBlockSize;
    QProcess *process;
    bool isRunning;
    QTimer timer;
    bool shouldQuit;
    bool debug;
};

#endif // SCREENREADERCLIENT_H
