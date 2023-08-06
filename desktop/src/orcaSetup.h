#ifndef ORCASETUP_H
#define ORCASETUP_H

#include <QString>


class OrcaSetup
{
public:
    OrcaSetup();
    void start();
    void restore();

private:
    void setupSpeechDispatcher();
    void copyPath(QString src, QString dst);
    void editSpeechDispatcher();
    void editOrcaSettings();
    void setAutoStart();
    void restoreSpeechDispatcher();
    void restoreOrcaSettings();
    void removeAutoStart();
};

#endif // ORCASETUP_H
