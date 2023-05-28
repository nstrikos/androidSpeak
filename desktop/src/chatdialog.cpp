#include <QtWidgets>

#include "chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    lineEdit->setFocusPolicy(Qt::StrongFocus);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    listWidget->setFocusPolicy(Qt::NoFocus);

    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(&client, SIGNAL(newMessage(QString,QString)),
            this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));

    myNickName = client.nickName();
    newParticipant(myNickName);
    tableFormat.setBorder(0);
    readSocket.bind(QHostAddress::LocalHost, 5824);
    connect(&readSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    if (!tripServer.listen(QHostAddress::LocalHost, 9090)) {
        qDebug() << "Failed to bind to port";
    }

    connect(&tripServer, SIGNAL(incomingText(QString)), this, SLOT(getText(QString)));
    clipBoard = QApplication::clipboard();
    connect(clipBoard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(clipBoardChanged(QClipboard::Mode)));
}

void ChatDialog::appendMessage(const QString &from, const QString &message)
{
    if (message == "ok")
        tripServer.sendMessage();

    QTextCursor cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDialog::returnPressed()
{
    QString text = lineEdit->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = textEdit->textColor();
        textEdit->setTextColor(Qt::red);
        textEdit->append(tr("! Unknown command: %1")
                         .arg(text.left(text.indexOf(' '))));
        textEdit->setTextColor(color);
    } else {
        client.sendMessage(text);
        appendMessage(myNickName, text);
    }

    lineEdit->clear();
}

void ChatDialog::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* %1 has joined").arg(nick));
    textEdit->setTextColor(color);
    listWidget->addItem(nick);
}

void ChatDialog::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = listWidget->findItems(nick,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* %1 has left").arg(nick));
    textEdit->setTextColor(color);
}

void ChatDialog::readData()
{
    QByteArray datagram;
    QString data;
    do {
        datagram.resize(readSocket.pendingDatagramSize());
        readSocket.readDatagram(datagram.data(), datagram.size());
    } while (readSocket.hasPendingDatagrams());

    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_3);
    in >> data;

    client.sendMessage(data);
    appendMessage("", data);
}

void ChatDialog::getText(QString text)
{
    client.sendMessage(text);
    appendMessage("", text);
}

void ChatDialog::clipBoardChanged(QClipboard::Mode mode)
{
        appendMessage("", clipBoard->text());
        client.sendMessage(clipBoard->text());
}
