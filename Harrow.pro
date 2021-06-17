QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    card.cpp \
    carddata.cpp \
    carddisplaylabel.cpp \
    cardframe.cpp \
    cardmat.cpp \
    cardpositioncontainer.cpp \
    cardspread.cpp \
    contextmenu.cpp \
    controlmat.cpp \
    drawframe.cpp \
    filedataparser.cpp \
    main.cpp \
    harrow.cpp \
    resultdatablock.cpp \
    resultdatamat.cpp \
    suit.cpp

HEADERS += \
    card.h \
    carddata.h \
    carddisplaylabel.h \
    cardframe.h \
    cardmat.h \
    cardpositioncontainer.h \
    cardspread.h \
    contextmenu.h \
    controlmat.h \
    defines.h \
    drawframe.h \
    filedataparser.h \
    harrow.h \
    resultdatablock.h \
    resultdatamat.h \
    suit.h

FORMS += \
    harrow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data.qrc \
    images.qrc

DISTFILES +=
