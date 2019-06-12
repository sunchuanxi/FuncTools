QT       += core gui concurrent
TEMPLATE = lib
VERSION = 3.0.1

DEFINES += VDGPU2_LIBRARY

TARGET      = vdGPU2

RESOURCES += vdGPU2.qrc

TRANSLATIONS +=vdgpu2_zh_CN.ts

DISTFILES += vdGPU2.json \
    vdgpu2.qml

HEADERS += vdGPU2plugin.h \
    vdGPU2.h \
    vdSGPU2_p.h

SOURCES += vdGPU2plugin.cpp \
    vdGPU2.cpp \
    vdGPU2_p.cpp \

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
