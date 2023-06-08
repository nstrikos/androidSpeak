#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include "comboBoxFiller.h"

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

    bool useClipboard() const;
    void setUseClipboard(bool newUseClipboard);

    QString speak() const;
    void setSpeak(const QString &newSpeak);

    QString stop() const;
    void setStop(const QString &newStop);

    QString activate() const;
    void setActivate(const QString &newActivate);

    bool speakCtrl() const;
    void setSpeakCtrl(bool newSpeakCtrl);

    bool speakAlt() const;
    void setSpeakAlt(bool newSpeakAlt);

    bool stopCtrl() const;
    void setStopCtrl(bool newStopCtrl);

    bool stopAlt() const;
    void setStopAlt(bool newStopAlt);

    bool activateCtrl() const;
    void setActivateCtrl(bool newActivateCtrl);

    bool activateAlt() const;
    void setActivateAlt(bool newActivateAlt);

private slots:
    void checkBox1Changed();
    void checkBox2Changed();
    void speakClearButtonPressed();
    void stopClearButtonPressed();
    void activateClearButtonPressed();
    void speakCheckBoxChanged();   
    void speakCtrlChanged();
    void speakAltChanged();
    void stopCheckBoxChanged();
    void stopCtrlChanged();
    void stopAltChanged();
    void activateCheckBoxChanged();
    void activateCtrlChanged();
    void activateAltChanged();
    void clipboardCheckBoxChanged();

private:
    Ui::OptionsDialog *ui;

    bool m_closeOnSystemTray;
    bool m_startMinimized;
    bool m_useClipboard;

    ComboBoxFiller *comboBoxFiller;
    QString m_speak;
    bool m_speakCtrl;
    bool m_speakAlt;
    QString m_stop;
    bool m_stopCtrl;
    bool m_stopAlt;
    QString m_activate;
    bool m_activateCtrl;
    bool m_activateAlt;
};

#endif // OPTIONSDIALOG_H
