#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T09:43:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UWVControl
TEMPLATE = app


SOURCES += main.cpp\
        maindialog.cpp \
    cameraview.cpp \
    tcpview.cpp \
    g.cpp \
    motorview.cpp \
    sensorview.cpp \
    platformview.cpp \
    controlview.cpp \
    PID.cpp \
    interpolation.cpp \
    localization.cpp \
    locationview.cpp \
    qcustomplot.cpp \
    qpaintlabel.cpp

HEADERS  += maindialog.h \
    cameraview.h \
    tcpview.h \
    g.h \
    motorview.h \
    sensorview.h \
    platformview.h \
    controlview.h \
    PID.h \
    interpolation.h \
    localization.h \
    locationview.h \
    qcustomplot.h \
    qpaintlabel.h

FORMS    += maindialog.ui \
    cameraview.ui \
    tcpview.ui \
    motorview.ui \
    sensorview.ui \
    platformview.ui \
    controlview.ui \
    locationview.ui

QT += network
QT += widgets printsupport

CONFIG += c++11

LIBS += -L$$PWD/NBaseUiKit/bin/ -lNBaseUiKit
include(NBaseUiKit/NBaseUiKit_inc.pri)

LIBS += -L$$PWD/HCNetSDK/bin/ -lHCNetSDK

include(HCNetSDK/HCNetSDK.pri)

include(opencv/opencv.pri)

INCLUDEPATH += Eigen/

OTHER_FILES +=

RESOURCES += \
    resources.qrc
