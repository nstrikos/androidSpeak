#include "optionsDialog.h"
#include "ui_optionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    connect(ui->checkBox, &QCheckBox::stateChanged, this, &OptionsDialog::checkBox1Changed);
    connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &OptionsDialog::checkBox2Changed);
    connect(ui->speakClearButton, &QPushButton::pressed, this, &OptionsDialog::speakClearButtonPressed);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &OptionsDialog::speakCheckBoxChanged);
    connect(ui->speakCtrlCheckBox, &QCheckBox::stateChanged, this, &OptionsDialog::speakCtrlChanged);
    connect(ui->speakAltCheckBox, &QCheckBox::stateChanged, this, &OptionsDialog::speakAltChanged);

    comboBoxFiller = new ComboBoxFiller();
    comboBoxFiller->fill(ui->comboBox);
    comboBoxFiller->fill(ui->comboBox_2);
    comboBoxFiller->fill(ui->comboBox_3);
}

OptionsDialog::~OptionsDialog()
{
    delete comboBoxFiller;
    delete ui;
}

bool OptionsDialog::closeOnSystemTray() const
{
    return m_closeOnSystemTray;
}

void OptionsDialog::setCloseOnSystemTray(bool newCloseOnSystemTray)
{
    m_closeOnSystemTray = newCloseOnSystemTray;
    ui->checkBox->setChecked(m_closeOnSystemTray);
}

bool OptionsDialog::startMinimized() const
{
    return m_startMinimized;
}

void OptionsDialog::setStartMinimized(bool newStartMinimized)
{
    m_startMinimized = newStartMinimized;
    ui->checkBox_2->setChecked(m_startMinimized);
}

void OptionsDialog::checkBox1Changed()
{
    m_closeOnSystemTray = ui->checkBox->isChecked();
}

void OptionsDialog::checkBox2Changed()
{
    m_startMinimized = ui->checkBox_2->isChecked();
}

void OptionsDialog::speakClearButtonPressed()
{
    ui->comboBox->setCurrentIndex(0);
    ui->speakCtrlCheckBox->setChecked(false);
    ui->speakAltCheckBox->setChecked(false);
}

void OptionsDialog::speakCheckBoxChanged()
{
    m_speak = ui->comboBox->currentText();
}

void OptionsDialog::speakCtrlChanged()
{
    m_speakCtrl = ui->speakCtrlCheckBox->isChecked();
}

void OptionsDialog::speakAltChanged()
{
    m_speakAlt = ui->speakAltCheckBox->isChecked();
}

bool OptionsDialog::speakAlt() const
{
    return m_speakAlt;
}

void OptionsDialog::setSpeakAlt(bool newSpeakAlt)
{
    m_speakAlt = newSpeakAlt;
}

bool OptionsDialog::speakCtrl() const
{
    return m_speakCtrl;
}

void OptionsDialog::setSpeakCtrl(bool newSpeakCtrl)
{
    m_speakCtrl = newSpeakCtrl;
}

QString OptionsDialog::activate() const
{
    return m_activate;
}

void OptionsDialog::setActivate(const QString &newActivate)
{
    m_activate = newActivate;
}

QString OptionsDialog::stop() const
{
    return m_stop;
}

void OptionsDialog::setStop(const QString &newStop)
{
    m_stop = newStop;
}

QString OptionsDialog::speak() const
{
    return m_speak;
}

void OptionsDialog::setSpeak(const QString &newSpeak)
{
    m_speak = newSpeak;
}

bool OptionsDialog::useClipboard() const
{
    return m_useClipboard;
}

void OptionsDialog::setUseClipboard(bool newUseClipboard)
{
    m_useClipboard = newUseClipboard;
    ui->clipboardCheckBox->setChecked(m_useClipboard);
}
