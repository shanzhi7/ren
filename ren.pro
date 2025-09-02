QT       += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    barrier.cpp \
    barriergeneration.cpp \
    boss.cpp \
    collidable.cpp \
    collisiondetector.cpp \
    healthbar.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    ruledialog.cpp

HEADERS += \
    barrier.h \
    barriergeneration.h \
    boss.h \
    collidable.h \
    collisiondetector.h \
    global.h \
    healthbar.h \
    mainwindow.h \
    player.h \
    ruledialog.h

FORMS += \
    mainwindow.ui \
    ruledialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    ren3/back.jpg

win32:RC_ICONS = windowIco.ico
