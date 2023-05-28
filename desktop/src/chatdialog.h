#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "ui_chatdialog.h"
#include "client.h"
#include <QUdpSocket>

#include "tripserver.h"
#include <QClipboard>

class ChatDialog : public QDialog, private Ui::ChatDialog
{
    Q_OBJECT

public:
    ChatDialog(QWidget *parent = 0);

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void readData();
    void getText(QString text);
    void clipBoardChanged(QClipboard::Mode mode);

private:
    Client client;
    QString myNickName;
    QTextTableFormat tableFormat;
    QUdpSocket readSocket;
    TripServer tripServer;
    QClipboard *clipBoard;
};

#endif
