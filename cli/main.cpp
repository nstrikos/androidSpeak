#include <QCoreApplication>

#include <QLocalSocket>

void sendMessage(QString text)
{
    QLocalSocket socket;
    socket.connectToServer("androidSpeak");

    if (!socket.waitForConnected()) {
        qDebug() << "Unable to connect to server:" << socket.errorString();
    }

    QString message = "text:" + text;
    qDebug() << "Sending message:" << message;

    socket.write(message.toUtf8());
    socket.flush();

    socket.disconnectFromServer();
    socket.waitForDisconnected();
}

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    QString inputText;

    if (argc < 2)
        QCoreApplication::quit();

    for (int i = 1; i < argc; i++) {
        QString arg = QString::fromUtf8(argv[i]);
        arg.replace("\"", "");
        arg.replace("(", "");
        arg.replace(")", "");
        inputText = inputText + arg.toUtf8() + " ";
    }

    sendMessage(inputText);

    return 0;
}
