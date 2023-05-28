#include "optionsDialog.h"
#include "ui_optionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    connect(ui->checkBox, &QCheckBox::stateChanged, this, &OptionsDialog::checkBox1Changed);
    connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &OptionsDialog::checkBox2Changed);
}

OptionsDialog::~OptionsDialog()
{
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
