QT += quick texttospeech

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    chat.cpp \
    client.cpp \
    connection.cpp \
    peermanager.cpp \
    server.cpp \
    texttospeech.cpp

android {
    QT += androidextras
    SOURCES += keepAwakeHelper.cpp


    HEADERS += keepAwakeHelper.h

}

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    chat.h \
    client.h \
    connection.h \
    peermanager.h \
    server.h \
    texttospeech.h

ANDROID_ABIS = armeabi-v7a arm64-v8a x86 x86_64

DISTFILES += \
    ../../../../media/data/nick/projects/omilo-android/android/AndroidManifest.xml \
    ../../../../media/data/nick/projects/omilo-android/android/build.gradle \
    ../../../../media/data/nick/projects/omilo-android/android/gradle/wrapper/gradle-wrapper.jar \
    ../../../../media/data/nick/projects/omilo-android/android/gradle/wrapper/gradle-wrapper.properties \
    ../../../../media/data/nick/projects/omilo-android/android/gradlew \
    ../../../../media/data/nick/projects/omilo-android/android/gradlew.bat \
    ../../../../media/data/nick/projects/omilo-android/android/res/values/libs.xml \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew \
    android/gradlew.bat \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/values/libs.xml \
    resources/audio.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
