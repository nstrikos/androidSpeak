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

signals:
    void sendText(QString text);
    void clipboardEnabled();
    void stopPressed();

protected:
    void run();

private:    
    QVector<HotKey> hotKeys;
    HotKey clipboardKey;
    HotKey stopKey;
    bool stopped = false;
};

#endif // HOTKEYTHREAD_H
