#include "fontsettingsdialog.h"
#include "ui_fontsettingsdialog.h"

FontSettingsDialog::FontSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontSettingsDialog)
{
    ui->setupUi(this);

    pointSize = qApp->font().pointSize();
    ui->spinBox->setMinimum(8);
    ui->spinBox->setMaximum(72);
    ui->spinBox->setValue(pointSize);

    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked()));
    connect(ui->boldCheckBox, &QCheckBox::clicked, this, &FontSettingsDialog::boldCheckBoxChanged);
}

FontSettingsDialog::~FontSettingsDialog()
{
    delete ui;
}

void FontSettingsDialog::okButtonClicked()
{
    font = ui->fontComboBox->currentFont();
    pointSize = ui->spinBox->value();
    this->accept();
}

void FontSettingsDialog::cancelButtonClicked()
{
    ui->spinBox->setValue(pointSize);
    this->reject();
}

void FontSettingsDialog::boldCheckBoxChanged()
{
    bold = ui->boldCheckBox->isChecked();
}

bool FontSettingsDialog::getBold() const
{
    return bold;
}

void FontSettingsDialog::setBold(bool newBold)
{
    bold = newBold;
    ui->boldCheckBox->setChecked(bold);
}

QFont FontSettingsDialog::getFont()
{
    return font;
}

void FontSettingsDialog::setFont(QFont font)
{
    ui->fontComboBox->setCurrentFont(font);
}

unsigned int FontSettingsDialog::getPointSize()
{
    return pointSize;
}

void FontSettingsDialog::setPointSize(int size)
{
    ui->spinBox->setValue(size);
}
