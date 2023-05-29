#include "shortcutWidget.h"
#include "ui_shortcutWidget.h"

#include <QDebug>

ShortcutWidget::ShortcutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShortcutWidget)
{
    ui->setupUi(this);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Create a widget to hold the layout
    contentWidget = new QWidget(scrollArea);

    // Create a layout for the content widget
    contentLayout = new QVBoxLayout(contentWidget);

    // Create and add labels dynamically
    for (int i = 0; i < 100; i++)
    {
        QHBoxLayout* innerLayout = new QHBoxLayout;
        innerLayouts.append(innerLayout);        

        QLabel* label = new QLabel(QString("Phrase %1").arg(i+1));
        labels.append(label);
        innerLayout->addWidget(label);

        QLineEdit *lineEdit = new QLineEdit();        
        innerLayout->addWidget(lineEdit);
        lineEdits.append(lineEdit);

        MyCombobox *combobox = new MyCombobox();
        innerLayout->addWidget(combobox);
        comboboxes.append(combobox);

        QCheckBox *checkbox1 = new QCheckBox("Ctrl");
        innerLayout->addWidget(checkbox1);
        ctrlBoxes.append(checkbox1);

        QCheckBox *checkbox2 = new QCheckBox("Alt");
        innerLayout->addWidget(checkbox2);
        altBoxes.append(checkbox2);

        MyPushButton *clearButton = new MyPushButton();
        clearButton->setCount(i);
        innerLayout->addWidget(clearButton);
        clearButtons.append(clearButton);
        connect(clearButton, &MyPushButton::clicked, clearButton, &MyPushButton::buttonClicked);
        connect(clearButton, &MyPushButton::updateCount, this, &ShortcutWidget::clearButtonPressed);

        contentLayout->addLayout(innerLayout);
    }

    // Set the content widget for the scroll area
    scrollArea->setWidget(contentWidget);

    // Create a button
    button = new QPushButton("OK", this);
    connect(button, &QPushButton::pressed, this, &ShortcutWidget::okButtonPressed);

    // Create a layout for the main window
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(button);
}

void ShortcutWidget::setKeys(QVector<HotKey> *hotkeys)
{
    for (int i = 1; i < hotkeys->size(); i++) {
        lineEdits.at(i)->setText(hotkeys->at(i).phrase);
        comboboxes.at(i)->setCurrentText(hotkeys->at(i).code);
        if (hotkeys->at(i).ctrl != 0)
            ctrlBoxes.at(i)->setChecked(true);
        else
            ctrlBoxes.at(i)->setChecked(false);
        if (hotkeys->at(i).alt != 0)
            altBoxes.at(i)->setChecked(true);
        else
            altBoxes.at(i)->setChecked(false);
    }
}

ShortcutWidget::~ShortcutWidget()
{
    for (int i = 0; i < hotkeys.size(); i++) {
        delete hotkeys.at(i);
    }

    for (int i = 0; i < innerLayouts.size(); i++)
        delete innerLayouts.at(i);
    for (int i = 0; i < labels.size(); i++)
        delete labels.at(i);
    for (int i = 0; i < lineEdits.size(); i++)
        delete lineEdits.at(i);
    for (int i = 0; i < comboboxes.size(); i++)
        delete comboboxes.at(i);
    for (int i = 0; i < ctrlBoxes.size(); i++)
        delete ctrlBoxes.at(i);
    for (int i = 0; i < altBoxes.size(); i++)
        delete altBoxes.at(i);
    for (int i = 0; i < clearButtons.size(); i++)
        delete clearButtons.at(i);
    delete button;
    delete contentLayout;
    delete contentWidget;
    delete scrollArea;
    delete mainLayout;
    delete ui;
}

void ShortcutWidget::okButtonPressed()
{
    this->hide();

    for (int i = 0; i < hotkeys.size(); i++) {
        delete hotkeys.at(i);
    }

    hotkeys.clear();


    for (int i = 0; i < 100; i++) {
        if (!lineEdits.at(i)->text().isEmpty() && !comboboxes.at(i)->currentText().isEmpty()) {
            HotKey *key = new HotKey();
            key->phrase = lineEdits.at(i)->text();
            key->setCode(comboboxes.at(i)->currentText());
            key->setCtrl(ctrlBoxes.at(i)->isChecked());
            key->setAlt(altBoxes.at(i)->isChecked());

            hotkeys.append(key);
        }
    }

    emit updateKeys(hotkeys);
}

void ShortcutWidget::clearButtonPressed(int i)
{
    QLineEdit *lineEdit = lineEdits.at(i);
    lineEdit->clear();
}
