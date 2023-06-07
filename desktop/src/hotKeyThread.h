#ifndef HOTKEYTHREAD_H
#define HOTKEYTHREAD_H

#include <QThread>
#include "hotKey.h"

class HotKeyThread : public QThread
{
    Q_OBJECT
public:
    HotKeyThread();
    ~HotKeyThread();
    void setStopped(bool stopped);
    void setKeys(QVector<HotKey> keys);

    void setClipboardKey(const HotKey &newClipboardKey);

    void setStopKey(const HotKey &newStopKey);

    void setActivateKey(const HotKey &newActivateKey);

signals:
    void sendText(QString text);
    void clipboardEnabled();
    void stopPressed();
    void activatePressed();

protected:
    void run();

private:    
    QVector<HotKey> hotKeys;
    HotKey clipboardKey;
    HotKey stopKey;
    HotKey activateKey;
    bool stopped = false;
};

#endif // HOTKEYTHREAD_H
