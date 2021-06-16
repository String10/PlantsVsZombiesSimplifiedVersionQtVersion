QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += statemachine

CONFIG += c++11

DESTDIR = $$PWD/../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bullet.cpp \
    catapultzombie.cpp \
    cherrybomb.cpp \
    coneheadzombie.cpp \
    garlic.cpp \
    interfacescene.cpp \
    jackintheboxzombie.cpp \
    main.cpp \
    newspaperzombie.cpp \
    normalzombie.cpp \
    peashooter.cpp \
    plantbase.cpp \
    plantcard.cpp \
    polevaultingzombie.cpp \
    pumpkin.cpp \
    repeater.cpp \
    snowpea.cpp \
    squash.cpp \
    sunflower.cpp \
    tallnut.cpp \
    wallnut.cpp \
    zombiebase.cpp

HEADERS += \
    bullet.h \
    catapultzombie.h \
    cherrybomb.h \
    coneheadzombie.h \
    garlic.h \
    interfacescene.h \
    jackintheboxzombie.h \
    newspaperzombie.h \
    normalzombie.h \
    peashooter.h \
    plantbase.h \
    plantcard.h \
    polevaultingzombie.h \
    pumpkin.h \
    repeater.h \
    snowpea.h \
    squash.h \
    sunflower.h \
    tallnut.h \
    wallnut.h \
    zombiebase.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
