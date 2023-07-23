#ifndef ORCASETUP_H
#define ORCASETUP_H

#include <QString>


class OrcaSetup
{
public:
    OrcaSetup();
    void start();

private:
    void setupSpeechDispatcher();
    void copyPath(QString src, QString dst);
    void editSpeechDispatcher();
    void editOrcaSettings();
    void setAutoStart();
};

#endif // ORCASETUP_H
