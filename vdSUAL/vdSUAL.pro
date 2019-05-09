QT       += core gui concurrent
TEMPLATE = lib
VERSION = 3.0.1

DEFINES += VDSUAL_LIBRARY

TARGET      = vdSUAL

RESOURCES += vdSUAL.qrc

TRANSLATIONS +=vdsual_zh_CN.ts

DISTFILES += vdSUAL.json \

HEADERS += vdSUALplugin.h \
    vdSUAL.h \
    vdSUAL_p.h

SOURCES += vdSUALplugin.cpp \
    vdSUAL.cpp \
    vdSUAL_p.cpp \

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    LIBS += -L$$PWD/../lib/ -lpgplugin
    PRE_TARGETDEPS += $$PWD/../lib/pgplugin.lib
    CONFIG(debug, debug|release) {
        DESTDIR = $$OUT_PWD/../Debug/plugins/mvtools/Defect_Detection
    } else {
        DESTDIR = "D:\Zigaa\B36\plugins\mvtools\Defect_Detection"
    }
    LIBS += -L$$(HALCONROOT)/lib/x64-win64/ -lhalconcpp
    INCLUDEPATH += $$(HALCONROOT)/include/
}
