#ifndef SHORTCUTWIDGET_H
#define SHORTCUTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include "comboBoxFiller.h"

#include "hotKey.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "myPushbutton.h"

namespace Ui {
class ShortcutWidget;
}

class ShortcutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShortcutWidget(QWidget *parent = nullptr);
    void setKeys(QVector<HotKey>*hotkeys);
    ~ShortcutWidget();

signals:
    void updateKeys(QVector<HotKey*>hotkeys);

private slots:
    void okButtonPressed();
    void clearButtonPressed(int i);

private:
    Ui::ShortcutWidget *ui;

    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QVBoxLayout* contentLayout;
    QVector<QHBoxLayout*>innerLayouts;
    QVector<QLabel*>labels;
    QPushButton *button;
    QVBoxLayout* mainLayout;

    QVector<QLineEdit*>lineEdits;
    QVector<QComboBox*>comboboxes;
    ComboBoxFiller *comboBoxFiller;
    QVector<QCheckBox*>ctrlBoxes;
    QVector<QCheckBox*>altBoxes;
    QVector<MyPushButton*>clearButtons;
    QVector<HotKey*>hotkeys;
};

#endif // SHORTCUTWIDGET_H
