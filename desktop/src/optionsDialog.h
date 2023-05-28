#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();

    bool closeOnSystemTray() const;
    void setCloseOnSystemTray(bool newCloseOnSystemTray);

    bool startMinimized() const;
    void setStartMinimized(bool newStartMinimized);

private slots:
    void checkBox1Changed();
    void checkBox2Changed();

private:
    Ui::OptionsDialog *ui;

    bool m_closeOnSystemTray;
    bool m_startMinimized;
};

#endif // OPTIONSDIALOG_H
