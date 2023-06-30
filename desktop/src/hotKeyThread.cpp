#include "hotKeyThread.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

HotKeyThread::HotKeyThread()
{
    Display *dpy = XOpenDisplay(0);
    Window root = DefaultRootWindow(dpy);

    XUngrabKey(dpy, AnyKey, AnyModifier, root);
}

void HotKeyThread::setStopped(bool stopped)
{
    Display *dpy = XOpenDisplay(0);
    Window root = DefaultRootWindow(dpy);

    Window          grab_window     = root;

    this->stopped = stopped;

    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask | Mod2Mask, grab_window);

    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask | Mod2Mask, grab_window);

    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask | Mod2Mask, grab_window);

    for (int i = 0; i < hotKeys.size(); i++) {
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | Mod2Mask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask | Mod2Mask, grab_window);
    }

    stopKey.keycode = 0;
    clipboardKey.keycode = 0;
    activateKey.keycode = 0;
}

void HotKeyThread::setKeys(QVector<HotKey> keys)
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    Window          grab_window     = root;

    for (int i = 0; i < hotKeys.size(); i++) {
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | Mod2Mask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask | Mod2Mask, grab_window);
    }

    hotKeys = keys;
}

HotKeyThread::~HotKeyThread()
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    Window          grab_window     = root;

    for (int i = 0; i < hotKeys.size(); i++) {
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | Mod2Mask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask | Mod2Mask, grab_window);
    }

    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask | Mod2Mask, grab_window);

    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask | Mod2Mask, grab_window);

    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask | Mod2Mask, grab_window);
}

void HotKeyThread::run()
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    XEvent      ev;
    KeySym key;
    char text[255];

    Window          grab_window     = root;
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

    stopped = false;

    for (int i = 0; i < hotKeys.size(); i++) {
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | Mod2Mask, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask | Mod2Mask, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
    }

    XGrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | Mod2Mask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask | Mod2Mask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);

    XGrabKey(dpy, stopKey.keycode, stopKey.modifiers, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, stopKey.keycode, stopKey.modifiers | Mod2Mask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask | Mod2Mask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);

    XGrabKey(dpy, activateKey.keycode, activateKey.modifiers, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, activateKey.keycode, activateKey.modifiers | Mod2Mask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask | Mod2Mask, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);

    XSelectInput(dpy, root, KeyPressMask);
    while(!stopped)
    {
        if (XPending(dpy) > 0) {

            XNextEvent(dpy, &ev);
            switch(ev.type)
            {
            case KeyPress:
                XLookupString(&ev.xkey,text,255,&key,0);

                for (int i = 0; i < hotKeys.size(); i++) {
                    if (key == hotKeys.at(i).keychar || key == hotKeys.at(i).keychar2) {
                        if (hotKeys.at(i).ctrl == (ev.xkey.state & ControlMask)) {
                            if (hotKeys.at(i).alt == ((ev.xkey.state & Mod1Mask))) {
                                emit sendText(hotKeys.at(i).phrase);
                            }
                        }
                    }
                }

                if (key == clipboardKey.keychar || key == clipboardKey.keychar2) {
                    if (clipboardKey.ctrl == (ev.xkey.state & ControlMask)) {
                        if (clipboardKey.alt == ((ev.xkey.state & Mod1Mask))) {
                            emit clipboardEnabled();
                        }
                    }
                }

                if (key == stopKey.keychar || key == stopKey.keychar2) {
                    if (stopKey.ctrl == (ev.xkey.state & ControlMask)) {
                        if (stopKey.alt == ((ev.xkey.state & Mod1Mask))) {
                            emit stopPressed();
                        }
                    }
                }

                if (key == activateKey.keychar || key == activateKey.keychar2) {
                    if (activateKey.ctrl == (ev.xkey.state & ControlMask)) {
                        if (activateKey.alt == ((ev.xkey.state & Mod1Mask))) {
                            emit activatePressed();
                        }
                    }
                }
            }
        }
        usleep(10000);
    }

    for (int i = 0; i < hotKeys.size(); i++) {
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | Mod2Mask, grab_window);
        XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask | Mod2Mask, grab_window);
    }

    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask | Mod2Mask, grab_window);

    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask | Mod2Mask, grab_window);

    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask | Mod2Mask, grab_window);

    XCloseDisplay(dpy);
}

void HotKeyThread::setActivateKey(const HotKey &newActivateKey)
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    Window          grab_window     = root;

    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, activateKey.keycode, activateKey.modifiers | LockMask | Mod2Mask, grab_window);

    activateKey = newActivateKey;
}

void HotKeyThread::setStopKey(const HotKey &newStopKey)
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    Window          grab_window     = root;

    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, stopKey.keycode, stopKey.modifiers | LockMask | Mod2Mask, grab_window);

    stopKey = newStopKey;
}

void HotKeyThread::setClipboardKey(const HotKey &newClipboardKey)
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    Window          grab_window     = root;

    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | Mod2Mask, grab_window);
    XUngrabKey(dpy, clipboardKey.keycode, clipboardKey.modifiers | LockMask | Mod2Mask, grab_window);
    clipboardKey = newClipboardKey;
}
