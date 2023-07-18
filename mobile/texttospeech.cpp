#include "texttospeech.h"
#include <QDebug>

TextToSpeech::TextToSpeech()
{
    m_volume = defaultVolume;
    m_rate = defaultRate;
    m_pitch = defaultPitch;

    m_speech = nullptr;

    m_speech = new QTextToSpeech(this);
    m_speech->setVolume(m_volume);
    m_speech->setRate(m_rate);
    m_speech->setPitch(m_pitch);

    connect(m_speech, &QTextToSpeech::stateChanged, this, &TextToSpeech::stateChanged);

    qDebug() << "TextToSpeech initialized";
}

TextToSpeech::~TextToSpeech()
{
    if (m_speech != nullptr)
        delete m_speech;

    qDebug() << "TextToSpeech deleted";
}

double TextToSpeech::volume() const
{
    return m_volume;
}

void TextToSpeech::setVolume(double volume)
{
    m_volume = volume / 100.0;
    m_speech->setVolume(m_volume);
}

double TextToSpeech::pitch() const
{
    return m_pitch;
}

void TextToSpeech::setPitch(double pitch)
{
    m_pitch = pitch / 100.0;
    m_speech->setPitch(m_pitch);
}

double TextToSpeech::rate() const
{
    return m_rate;
}

void TextToSpeech::setRate(double rate)
{
    m_rate = rate / 100.0;
    m_speech->setRate(m_rate);
}

void TextToSpeech::quit()
{
    if (m_speech != nullptr)
        m_speech->stop();
}

void TextToSpeech::speak(QString text)
{
    m_speech->say(text);
}

void TextToSpeech::stop()
{
    m_speech->stop();
}

QTextToSpeech::State TextToSpeech::state()
{
    return m_speech->state();
}
