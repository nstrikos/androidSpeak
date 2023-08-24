#ifndef SCREENREADERDIALOG_H
#define SCREENREADERDIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QTimer>
#include <QKeyEvent>

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
};

#endif // SCREENREADERDIALOG_H
