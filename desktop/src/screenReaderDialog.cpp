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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    audioOutput = nullptr;
#endif
}

ScreenReaderDialog::~ScreenReaderDialog()
{
    if (player != nullptr)
        delete player;
    if (timer != nullptr)
        delete timer;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (audioOutput != nullptr)
        delete audioOutput;
#endif

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
    if (e->key() == Qt::Key_Y)
        emit yes();    
    else if (e->key() == Qt::Key_N)
        emit no();    
}

void ScreenReaderDialog::playSound()
{
    if (player == nullptr)
        player = new QMediaPlayer;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (audioOutput == nullptr)
        audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl(audioFile));
    player->play();
#else
    player->setMedia(QUrl(audioFile));
    player->play();
#endif
}
