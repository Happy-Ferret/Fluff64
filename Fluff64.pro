#-------------------------------------------------
#
# Project created by QtCreator 2018-02-03T17:17:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fluff64
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    source/limage/limage.cpp \
    imageworker.cpp \
    source/lcolorlist.cpp \
    source/util/util.cpp \
    data.cpp \
    source/toolbox.cpp \
    source/toolboxitem.cpp \
    source/workerthread.cpp \
    source/limage/limageqimage.cpp \
    source/limage/multicolorimage.cpp \
    source/limage/limagefactory.cpp

HEADERS  += mainwindow.h \
    source/limage/limage.h \
    imageworker.h \
    source/lcolorlist.h \
    source/util/random.h \
    source/util/util.h \
    data.h \
    source/toolbox.h \
    source/toolboxitem.h \
    source/workerthread.h \
    source/limage/limageqimage.h \
    source/limage/multicolorimage.h \
    source/limage/limagefactory.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
