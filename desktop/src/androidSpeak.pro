QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
HEADERS       = chatdialog.h \
                client.h \
                clientsocket.h \
                connection.h \
                hotKey.h \
                hotKeyThread.h \
                mainwindow.h \
                myCombobox.h \
                myPushbutton.h \
                mythread.h \
                optionsDialog.h \
                peermanager.h \
                server.h \
                shortcutWidget.h \
                tripserver.h
SOURCES       = chatdialog.cpp \
                client.cpp \
                clientsocket.cpp \
                connection.cpp \
                hotKey.cpp \
                hotKeyThread.cpp \
                main.cpp \
                mainwindow.cpp \
                myCombobox.cpp \
                myPushbutton.cpp \
                mythread.cpp \
                optionsDialog.cpp \
                peermanager.cpp \
                server.cpp \
                shortcutWidget.cpp \
                tripserver.cpp
FORMS         = chatdialog.ui \
    mainwindow.ui \
    optionsDialog.ui \
    shortcutWidget.ui

QT           += network widgets
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

LIBS += -lX11

RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
