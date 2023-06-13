#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include <QObject>
#include <QTextToSpeech>
#include <QString>
#include <QVector>

const double defaultVolume = 0.5;
const double defaultRate = 0.0;
const double defaultPitch = 0.0;

class TextToSpeech : public QObject
{
    Q_OBJECT

public:
    TextToSpeech();
    ~TextToSpeech();

    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(double pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)

    Q_INVOKABLE void speak(QString text);
    Q_INVOKABLE void stop();

    QTextToSpeech::State state();

    double volume() const;
    void setVolume(double volume);

    double pitch() const;
    void setPitch(double pitch);

    double rate() const;
    void setRate(double rate);

signals:
    void stateChanged();
    void volumeChanged();
    void pitchChanged();
    void rateChanged();

private:
    QTextToSpeech *m_speech;

    double m_volume;
    double m_pitch;
    double m_rate;
};

#endif // TEXTTOSPEECH_H
