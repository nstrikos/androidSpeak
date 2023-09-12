#ifndef SCREENREADERDIALOG_H
#define SCREENREADERDIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QTimer>
#include <QKeyEvent>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioOutput>
#endif


namespace Ui {
class ScreenReaderDialog;
}

class ScreenReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenReaderDialog(QString text, QString audioFile, QWidget *parent = nullptr);
    ~ScreenReaderDialog();
    void start();
    void stop();

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void yes();
    void no();

private slots:
    void playSound();

private:
    Ui::ScreenReaderDialog *ui;
    QMediaPlayer *player;
    QTimer *timer;
    QString audioFile;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QAudioOutput *audioOutput;
#endif
};

#endif // SCREENREADERDIALOG_H
