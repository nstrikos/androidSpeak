#include "screenReaderDialog.h"
#include "ui_screenReaderDialog.h"

ScreenReaderDialog::ScreenReaderDialog(QString text, QString audioFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScreenReaderDialog),
    audioFile(audioFile)
{
    ui->setupUi(this);
    this->setModal(true);
    player = nullptr;
    timer = nullptr;
    ui->textEdit->setText(text);
}

ScreenReaderDialog::~ScreenReaderDialog()
{
    delete ui;
}

void ScreenReaderDialog::start()
{
    show();
    playSound();

    if (timer == nullptr)
        timer = new QTimer();

    timer->setInterval(12000);
    connect(timer, &QTimer::timeout, this, &ScreenReaderDialog::playSound);
    timer->start();
}

void ScreenReaderDialog::stop()
{
    if (player != nullptr)
        player->stop();
    if (timer != nullptr)
        timer->stop();
}

void ScreenReaderDialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Y) {
        //this->accept();
        emit yes();
    }
    else if (e->key() == Qt::Key_N) {
        //this->reject();
        emit no();
    }
}

void ScreenReaderDialog::playSound()
{
    if (player == nullptr)
        player = new QMediaPlayer;

    player->setMedia(QUrl(audioFile));
    player->play();
}
