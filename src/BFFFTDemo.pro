#-------------------------------------------------
#
# Project created by QtCreator 2012-12-14T08:01:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BFFFTDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bfconnectionthread.cpp \
    spectrographwidget.cpp \
    frequencyspectrum.cpp \
    spectrumanalyser.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    bfconnectionthread.h \
    spectrographwidget.h \
    frequencyspectrum.h \
    spectrumanalyser.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
