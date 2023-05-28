#include "hotKey.h"

#include "mainwindow.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

HotKey::HotKey()
{
    alt = false;
    ctrl = false;
    modifiers = 0;
}

void HotKey::setCode(QString shortcut)
{
    code = shortcut;
    updateKeyCode();
}

void HotKey::setAlt(bool alt)
{
    if (alt)
        this->alt = Mod1Mask;
    else
        this->alt = 0;

    updateModifiers();
}

void HotKey::setCtrl(bool ctrl)
{
    if (ctrl)
        this->ctrl = ControlMask;
    else
        this->ctrl = 0;

    updateModifiers();
}

void HotKey::updateKeyCode()
{
    if (code == "A") {
        keychar = XK_A;
        keychar2 = XK_a;
    } else if (code == "B") {
        keychar = XK_B;
        keychar2 = XK_b;
    } else if (code == "C") {
        keychar = XK_C;
        keychar2 = XK_c;
    } else if (code == "D") {
        keychar = XK_D;
        keychar2 = XK_d;
    } else if (code == "E") {
        keychar = XK_E;
        keychar2 = XK_e;
    } else if (code == "F") {
        keychar = XK_F;
        keychar2 = XK_f;
    } else if (code == "G") {
        keychar = XK_G;
        keychar2 = XK_g;
    } else if (code == "H") {
        keychar = XK_H;
        keychar2 = XK_h;
    } else if (code == "I") {
        keychar = XK_I;
        keychar2 = XK_i;
    } else if (code == "J") {
        keychar = XK_J;
        keychar2 = XK_j;
    } else if (code == "K") {
        keychar = XK_K;
        keychar2 = XK_k;
    } else if (code == "L") {
        keychar = XK_L;
        keychar2 = XK_l;
    } else if (code == "M") {
        keychar = XK_M;
        keychar2 = XK_m;
    } else if (code == "N") {
        keychar = XK_N;
        keychar2 = XK_n;
    } else if (code == "O") {
        keychar = XK_O;
        keychar2 = XK_o;
    } else if (code == "P") {
        keychar = XK_P;
        keychar2 = XK_p;
    } else if (code == "Q") {
        keychar = XK_Q;
        keychar2 = XK_q;
    } else if (code == "R") {
        keychar = XK_R;
        keychar2 = XK_r;
    } else if (code == "S") {
        keychar = XK_S;
        keychar2 = XK_s;
    } else if (code == "T") {
        keychar = XK_T;
        keychar2 = XK_t;
    } else if (code == "U") {
        keychar = XK_U;
        keychar2 = XK_u;
    } else if (code == "V") {
        keychar = XK_V;
        keychar2 = XK_v;
    } else if (code == "W") {
        keychar = XK_W;
        keychar2 = XK_w;
    } else if (code == "X") {
        keychar = XK_X;
        keychar2 = XK_x;
    } else if (code == "Y") {
        keychar = XK_Y;
        keychar2 = XK_y;
    } else if (code == "Z") {
        keychar = XK_Z;
        keychar2 = XK_z;
    } else if (code == "0") {
        keychar = XK_0;
        keychar2 = XK_0;
    } else if (code == "1") {
        keychar = XK_1;
        keychar2 = XK_1;
    } else if (code == "2") {
        keychar = XK_2;
        keychar2 = XK_2;
    } else if (code == "3") {
        keychar = XK_3;
        keychar2 = XK_3;
    } else if (code == "4") {
        keychar = XK_4;
        keychar2 = XK_4;
    } else if (code == "5") {
        keychar = XK_5;
        keychar2 = XK_5;
    } else if (code == "6") {
        keychar = XK_6;
        keychar2 = XK_6;
    } else if (code == "7") {
        keychar = XK_7;
        keychar2 = XK_7;
    } else if (code == "8") {
        keychar = XK_8;
        keychar2 = XK_8;
    } else if (code == "9") {
        keychar = XK_9;
        keychar2 = XK_9;
    } else if (code == "F1") {
        keychar = XK_F1;
        keychar2 = XK_F1;
    } else if (code == "F2") {
        keychar = XK_F2;
        keychar2 = XK_F2;
    } else if (code == "F3") {
        keychar = XK_F3;
        keychar2 = XK_F3;
    } else if (code == "F4") {
        keychar = XK_F4;
        keychar2 = XK_F4;
    } else if (code == "F5") {
        keychar = XK_F5;
        keychar2 = XK_F5;
    } else if (code == "F6") {
        keychar = XK_F6;
        keychar2 = XK_F6;
    } else if (code == "F7") {
        keychar = XK_F7;
        keychar2 = XK_F7;
    } else if (code == "F8") {
        keychar = XK_F8;
        keychar2 = XK_F8;
    } else if (code == "F9") {
        keychar = XK_F9;
        keychar2 = XK_F9;
    } else if (code == "F10") {
        keychar = XK_F10;
        keychar2 = XK_F10;
    } else if (code == "F11") {
        keychar = XK_F11;
        keychar2 = XK_F11;
    } else if (code == "F12") {
        keychar = XK_F12;
        keychar2 = XK_F12;
    }

     Display *dpy = XOpenDisplay(0);
     keycode = XKeysymToKeycode(dpy, keychar);
}

void HotKey::updateModifiers()
{
     modifiers = alt | ctrl;
}
