#ifndef FONTSETTINGSDIALOG_H
#define FONTSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class FontSettingsDialog;
}

class FontSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontSettingsDialog(QWidget *parent = 0);
    ~FontSettingsDialog();
    QFont getFont();
    void setFont(QFont font);
    unsigned int getPointSize();
    void setPointSize(int size);

    bool getBold() const;
    void setBold(bool newBold);

private slots:
    void okButtonClicked();
    void cancelButtonClicked();
    void boldCheckBoxChanged();

private:
    Ui::FontSettingsDialog *ui;
    QFont font;
    unsigned int pointSize;
    bool bold;
};

#endif // FONTSETTINGSDIALOG_H
