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
    connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, &OptionsDialog::stopCheckBoxChanged);
    connect(ui->stopCtrlCheckBox, &QCheckBox::stateChanged, this, &OptionsDialog::stopCtrlChanged);
    connect(ui->stopAltCheckBox, &QCheckBox::stateChanged, this, &OptionsDialog::stopAltChanged);
    connect(ui->stopClearButton, &QPushButton::pressed, this, &OptionsDialog::stopClearButtonPressed);
    connect(ui->comboBox_3, &QComboBox::currentTextChanged, this, &OptionsDialog::activateCheckBoxChanged);
    connect(ui->activateCtrlCheckBox, &QCheckBox::stateChanged, this, &OptionsDialog::activateCtrlChanged);
    connect(ui->activateAltCheckBox, &QCheckBox::stateChanged, this, &OptionsDialog::activateAltChanged);
    connect(ui->activateClearButton, &QPushButton::pressed, this, &OptionsDialog::activateClearButtonPressed);
    connect(ui->clipboardCheckBox, &QCheckBox::stateChanged, this, &OptionsDialog::clipboardCheckBoxChanged);

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

void OptionsDialog::stopClearButtonPressed()
{
    ui->comboBox_2->setCurrentIndex(0);
    ui->stopCtrlCheckBox->setChecked(false);
    ui->stopAltCheckBox->setChecked(false);
}

void OptionsDialog::activateClearButtonPressed()
{
    ui->comboBox_3->setCurrentIndex(0);
    ui->activateCtrlCheckBox->setChecked(false);
    ui->activateAltCheckBox->setChecked(false);
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

void OptionsDialog::stopCheckBoxChanged()
{
    m_stop = ui->comboBox_2->currentText();
}

void OptionsDialog::stopCtrlChanged()
{
    m_stopCtrl = ui->stopCtrlCheckBox->isChecked();
}

void OptionsDialog::stopAltChanged()
{
    m_stopAlt = ui->stopAltCheckBox->isChecked();
}

void OptionsDialog::activateCheckBoxChanged()
{
    m_activate = ui->comboBox_3->currentText();
}

void OptionsDialog::activateCtrlChanged()
{
    m_activateCtrl = ui->activateCtrlCheckBox->isChecked();
}

void OptionsDialog::activateAltChanged()
{
    m_activateAlt = ui->activateAltCheckBox->isChecked();
}

void OptionsDialog::clipboardCheckBoxChanged()
{
    m_useClipboard = ui->clipboardCheckBox->isChecked();
}

bool OptionsDialog::activateAlt() const
{
    return m_activateAlt;
}

void OptionsDialog::setActivateAlt(bool newActivateAlt)
{
    m_activateAlt = newActivateAlt;
    ui->activateAltCheckBox->setChecked(m_activateAlt);
}

bool OptionsDialog::activateCtrl() const
{
    return m_activateCtrl;
}

void OptionsDialog::setActivateCtrl(bool newActivateCtrl)
{
    m_activateCtrl = newActivateCtrl;
    ui->activateCtrlCheckBox->setChecked(m_activateCtrl);
}

bool OptionsDialog::stopAlt() const
{
    return m_stopAlt;
}

void OptionsDialog::setStopAlt(bool newStopAlt)
{
    m_stopAlt = newStopAlt;
    ui->stopAltCheckBox->setChecked(m_stopAlt);
}

bool OptionsDialog::stopCtrl() const
{
    return m_stopCtrl;
}

void OptionsDialog::setStopCtrl(bool newStopCtrl)
{
    m_stopCtrl = newStopCtrl;
    ui->stopCtrlCheckBox->setChecked(m_stopCtrl);
}

bool OptionsDialog::speakAlt() const
{
    return m_speakAlt;
}

void OptionsDialog::setSpeakAlt(bool newSpeakAlt)
{
    m_speakAlt = newSpeakAlt;
    ui->speakAltCheckBox->setChecked(m_speakAlt);
}

bool OptionsDialog::speakCtrl() const
{
    return m_speakCtrl;
}

void OptionsDialog::setSpeakCtrl(bool newSpeakCtrl)
{
    m_speakCtrl = newSpeakCtrl;
    ui->speakCtrlCheckBox->setChecked(m_speakCtrl);
}

QString OptionsDialog::activate() const
{
    return m_activate;
}

void OptionsDialog::setActivate(const QString &newActivate)
{
    m_activate = newActivate;
    ui->comboBox_3->setCurrentText(m_activate);
}

QString OptionsDialog::stop() const
{
    return m_stop;
}

void OptionsDialog::setStop(const QString &newStop)
{
    m_stop = newStop;
    ui->comboBox_2->setCurrentText(m_stop);
}

QString OptionsDialog::speak() const
{
    return m_speak;
}

void OptionsDialog::setSpeak(const QString &newSpeak)
{
    m_speak = newSpeak;
    ui->comboBox->setCurrentText(m_speak);
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
