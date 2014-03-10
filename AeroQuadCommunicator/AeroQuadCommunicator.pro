#-------------------------------------------------
#
# Project created by QtCreator 2014-01-06T21:59:44
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport printsupport

TARGET = AeroQuadCommunicator
TEMPLATE = app


SOURCES += main.cpp\
    communicator.cpp \
    panel/config/panel_config.cpp \
    panel/connect/menu_connect.cpp \
    panel/firmware/panel_firmware.cpp \
    panel/monitor/panel_monitor.cpp \
    panel/plot/panel_plot.cpp \
    panel/plot/qcustomplot.cpp \
    panel/route/panel_route.cpp \
    communication/communication.cpp \
    communication/communication_serial.cpp \
    #panel/route/positionsimulator.cpp \
    panel/route/route.cpp \
    panel/route/vehicle.cpp \
    panel/example/panel_example.cpp \
    panel/calibration/panel_calibrate.cpp

HEADERS  += communicator.h \
    panel/config/panel_config.h \
    panel/connect/menu_connect.h \
    panel/firmware/panel_firmware.h \
    panel/monitor/panel_monitor.h \
    panel/plot/panel_plot.h \
    panel/plot/qcustomplot.h \
    panel/route/panel_route.h \
    communication/communication.h \
    communication/communication_serial.h \
    #panel/route/positionsimulator.h \
    panel/route/route.h \
    panel/route/vehicle.h \
    panel/example/panel_example.h \
    panel/calibration/panel_calibrate.h

FORMS    += communicator.ui \
    panel/config/panel_config.ui \
    panel/connect/menu_connect.ui \
    panel/firmware/panel_firmware.ui \
    panel/monitor/panel_monitor.ui \
    panel/plot/panel_plot.ui \
    panel/route/panel_route.ui \
    panel/example/panel_example.ui \
    panel/calibration/panel_calibrate.ui

RESOURCES += \
    commuicator.qrc \

OTHER_FILES += \
    panel/config/panel_config.xml \
    panel/plot/panel_plot.xml \
    panel/route/panel_route.xml

win32 {
    ## Windows common build here
    !contains(QMAKE_HOST.arch, x86_64) {
        message("x86 build")
        LIBS += $$PWD/marble/libmarblewidget.dll
                #$$PWD/marble/libastro.dll
        #LIBS = $$quote(C:\Qt\Qt5.2.0\5.2.0\mingw48_32\lib\libmarblewidget.dll)
    }
    else {
        message("x86_64 build")
        LIBS += marblewidget.lib
    }
}

mac {
    LIBS = /Users/Ted/Development/Marble/build/mac/src/lib/marble/libmarblewidget.0.16.80.dylib
}
