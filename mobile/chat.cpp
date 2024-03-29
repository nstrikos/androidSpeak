#include "chat.h"

#include <QDebug>

Chat::Chat(TextToSpeech &textToSpeech): m_textToSpeech(textToSpeech)
{
    connect(&client, SIGNAL(newMessage(QString,QString)), this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(connectedToPeer()), this, SLOT(connectedToPeer()));
    connect(&client, SIGNAL(disconnectedFromPeer()), this, SLOT(disconnectedFromPeer()));

    connect(&m_textToSpeech, &TextToSpeech::stateChanged, this, &Chat::textToSpeechFinished);
}

void Chat::appendMessage(const QString &from, const QString &message)
{
    Q_UNUSED(from);

    if (message == "command-stop") {
        m_textToSpeech.stop();
    } else if (message.startsWith("command-text:")) {
        QString cmd = "command-text:";
        int length = cmd.length();
        m_text = message.right(message.length() - length);
        if (m_textToSpeech.state() == QTextToSpeech::Ready)
            m_speaking = false;
        else
            m_speaking = true;

        m_textToSpeech.speak(m_text);
    } else if (message.startsWith("command-rate:")) {
        QString cmd = "command-rate:";
        int length = cmd.length();
        QString rate = message.right(message.length() - length);
        m_textToSpeech.setRate(rate.toDouble());
    } else if (message.startsWith("command-pitch:")) {
        QString cmd = "command-pitch:";
        int length = cmd.length();
        QString pitch = message.right(message.length() - length);
        m_textToSpeech.setPitch(pitch.toDouble());
    }
}

void Chat::connectedToPeer()
{
    emit connected();
}

void Chat::disconnectedFromPeer()
{
    emit disconnected();
}

void Chat::textToSpeechFinished()
{
    if (m_textToSpeech.state() == QTextToSpeech::Ready) {
        if (m_speaking == false)
            client.sendMessage("command-finished:" + m_text);
    }
    m_speaking = false;
}
