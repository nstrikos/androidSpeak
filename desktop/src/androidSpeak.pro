QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
HEADERS       = \
                client.h \
                clientsocket.h \
                comboBoxFiller.h \
                connection.h \
                fontsettingsdialog.h \
                hotKey.h \
                hotKeyThread.h \
                mainwindow.h \
                myPushbutton.h \
                mythread.h \
                optionsDialog.h \
                orcaSetup.h \
                peermanager.h \
                screenReaderClient.h \
                screenReaderDialog.h \
                server.h \
                shortcutWidget.h \
                tripserver.h
SOURCES       = \
                client.cpp \
                clientsocket.cpp \
                comboBoxFiller.cpp \
                connection.cpp \
                fontsettingsdialog.cpp \
                hotKey.cpp \
                hotKeyThread.cpp \
                main.cpp \
                mainwindow.cpp \
                myPushbutton.cpp \
                mythread.cpp \
                optionsDialog.cpp \
                orcaSetup.cpp \
                peermanager.cpp \
                screenReaderClient.cpp \
                screenReaderDialog.cpp \
                server.cpp \
                shortcutWidget.cpp \
                tripserver.cpp
FORMS         = \
    fontsettingsdialog.ui \
    mainwindow.ui \
    optionsDialog.ui \
    screenReaderDialog.ui \
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
