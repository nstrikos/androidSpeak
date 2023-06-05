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
    this->stopped = stopped;
}

void HotKeyThread::setKeys(QVector<HotKey> keys)
{
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
                    if ( (key == hotKeys.at(i).keychar || key == hotKeys.at(i).keychar2) &&
                        ev.xkey.state & hotKeys.at(i).modifiers) {
                            emit sendText(hotKeys.at(i).phrase);
                    }
                }

                if ( (key == clipboardKey.keychar || key == clipboardKey.keychar2) &&
                    ev.xkey.state & clipboardKey.modifiers) {
                    emit clipboardEnabled();
                }

            }
        }
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

    XCloseDisplay(dpy);
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
