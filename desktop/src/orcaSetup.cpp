#include "orcaSetup.h"

#include <QDebug>

#include <QDir>

OrcaSetup::OrcaSetup()
{

}

void OrcaSetup::start()
{
    qDebug() << "Setup orca";
    setupSpeechDispatcher();
    editSpeechDispatcher();
    editOrcaSettings();
    setAutoStart();
}

void OrcaSetup::restore()
{
    restoreSpeechDispatcher();
    restoreOrcaSettings();
    removeAutoStart();
}

void OrcaSetup::setupSpeechDispatcher()
{
    QString speechDispatcherDir = QDir::homePath() + "/.config/speech-dispatcher";

    if (QDir(speechDispatcherDir).exists()) {
        qDebug() << "speech dispatcher directory exists";
        //editSpeechDispatcher();

    } else {
        qDebug() << "Speech dispatcher directory does not exist." << speechDispatcherDir;
        if (QDir().mkdir(speechDispatcherDir)) {
            qDebug() << "Speech dispatcher directory created.";
            copyPath("/etc/speech-dispatcher", speechDispatcherDir);
        } else {
            qDebug() << "Cannot create directory: " << speechDispatcherDir;
        }

    }

    QString modulesPath = speechDispatcherDir + "/modules";
    if (!QDir(modulesPath).exists()) {
        if(QDir().mkdir(modulesPath)) {
            qDebug() << "Created speech dispatcher modules directory";
            QFile::copy("/usr/share/androidSpeak/androidSpeak.conf", modulesPath + "/androidSpeak.conf");
        } else {
            qDebug() << "Cannot create speech dispatcher modules directory";
        }
    } else {
        QFile::copy("/usr/share/androidSpeak/androidSpeak.conf", modulesPath + "/androidSpeak.conf");
    }
}

void OrcaSetup::copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}

void OrcaSetup::editSpeechDispatcher()
{
    QString filename = QDir::homePath() + "/.config/speech-dispatcher/speechd.conf";
    QString ext = ".bak";

    QFile::copy(filename, filename + ext);

    QFile::remove(filename);

    QFile file(filename + ext);
    QString text;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            text = stream.readAll();
        }
    }
    file.close();

    int n = text.lastIndexOf("AddModule \"espeak\"");
    int i = text.indexOf("\n", n);
    text = text.insert(i, "\nAddModule \"androidSpeak\" \"sd_generic\" \"androidSpeak.conf\"\n");

    n = text.indexOf("\nDefaultModule");

    //Two possibilities: there is #DefaultModule commented or there is DefaultModule

    if (n != -1) {
        i = n - 1;
        if (text.at(i) != "#")
        {
            text = text.insert(n+1, "#");
        }
        i = text.indexOf("\n", n + 1);
        text = text.insert(i, "\nDefaultModule androidSpeak\n");
    } else {
        n = text.indexOf("#DefaultModule");
        i = text.indexOf("\n", n);
        text = text.insert(i, "\nDefaultModule androidSpeak\n");
    }

    QFile dst(filename);
    if (dst.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream stream(&dst);
        stream << text;
    }
    dst.close();
}

void OrcaSetup::editOrcaSettings()
{
    QString filename = QDir::homePath() + "/.local/share/orca/user-settings.conf";
    QFile::copy(filename, filename + ".bak");
    QFile::remove(filename);

    QFile file(filename + ".bak");
    QString text;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            text = stream.readAll();
        }
    }
    file.close();

    int n = text.indexOf("\"speechServerInfo");
    int i = text.indexOf(",", n);
    QString line = text.mid(n, i - n);
    qDebug() << n << i << line;

    text = text.remove(n, i-n);
    text = text.insert(n, "\"speechServerInfo\": [\"androidSpeak\", \"androidSpeak\" ]");

    QFile dst(filename);
    if (dst.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream stream(&dst);
        stream << text;
    }
    dst.close();
}

void OrcaSetup::setAutoStart()
{
    QString dirName = QDir::homePath() + "/.config/autostart/";

    if (!QDir(dirName).exists()) {
        QDir().mkdir(dirName);
    }

    QFile::copy("/usr/share/applications/androidSpeak.desktop", dirName + "androidSpeak.desktop");
}

void OrcaSetup::restoreSpeechDispatcher()
{
    QString filename = QDir::homePath() + "/.config/speech-dispatcher/speechd.conf";
    QString ext = ".bak";

    if (QFile::exists(filename + ext)) {

        QFile::remove(filename);

        QFile::copy(filename + ext, filename);
        QFile::remove(filename + ext);

        QFile::remove(QDir::homePath() + "/.config/speech-dispatcher/modules/androidSpeak.conf");
    } else {
        QDir(QDir::homePath() + "/.config/speech-dispatcher/").removeRecursively();
    }
}

void OrcaSetup::restoreOrcaSettings()
{
    QString filename = QDir::homePath() + "/.local/share/orca/user-settings.conf";

    if (QFile::exists(filename + ".bak")) {
        qDebug() << "Found orca backup file";
        QFile::remove(filename);
        QFile::copy(filename + ".bak", filename);
        QFile::remove(filename + ".bak");
    }
}

void OrcaSetup::removeAutoStart()
{
    QString dirName = QDir::homePath() + "/.config/autostart/androidSpeak.desktop";
    QFile::remove(dirName);
}
