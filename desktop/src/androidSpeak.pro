QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
HEADERS       = chatdialog.h \
                chatdialog.h \
                client.h \
                client.h \
                clientsocket.h \
                clientsocket.h \
                connection.h \
                connection.h \
                hotKey.h \
                hotKey.h \
                hotKeyThread.h \
                hotKeyThread.h \
                mainwindow.h \
                myCombobox.h \
                myCombobox.h \
                myPushbutton.h \
                myPushbutton.h \
                mythread.h \
                mythread.h \
                optionsDialog.h \
                optionsDialog.h \
                peermanager.h \
                peermanager.h \
                server.h \
                server.h \
                shortcutWidget.h \
                shortcutWidget.h \
                tripserver.h \
                tripserver.h
SOURCES       = chatdialog.cpp \
                chatdialog.cpp \
                client.cpp \
                client.cpp \
                clientsocket.cpp \
                clientsocket.cpp \
                connection.cpp \
                connection.cpp \
                hotKey.cpp \
                hotKey.cpp \
                hotKeyThread.cpp \
                hotKeyThread.cpp \
                main.cpp \
                mainwindow.cpp \
                myCombobox.cpp \
                myCombobox.cpp \
                myPushbutton.cpp \
                myPushbutton.cpp \
                mythread.cpp \
                mythread.cpp \
                optionsDialog.cpp \
                optionsDialog.cpp \
                peermanager.cpp \
                peermanager.cpp \
                server.cpp \
                server.cpp \
                shortcutWidget.cpp \
                shortcutWidget.cpp \
                tripserver.cpp \
                tripserver.cpp
FORMS         = chatdialog.ui \
    chatdialog.ui \
    mainwindow.ui \
    optionsDialog.ui \
    optionsDialog.ui \
    shortcutWidget.ui \
    shortcutWidget.ui
QT           += network widgets
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

LIBS += -lX11

RESOURCES += \
    resources.qrc \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
