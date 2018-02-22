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

win32-g++ {
  QMAKE_CXXFLAGS += -fopenmp
  LIBS += -fopenmp
}
win32-msvc*{
  QMAKE_CXXFLAGS += -openmp
  LIBS += -openmp
}

SOURCES += main.cpp\
        mainwindow.cpp \
    imageworker.cpp \
    #source/lcolorlist.cpp \
    source/toolbox.cpp \
    source/toolboxitem.cpp \
    source/workerthread.cpp \
#    source/limage/limage.cpp \
    #source/limage/limageqimage.cpp \
    #source/limage/multicolorimage.cpp \
    #source/limage/limagefactory.cpp \
    #source/limage/standardcolorimage.cpp \
    imageeditor.cpp \
    dialognewimage.cpp \
    dialogimport.cpp \
#    source/limage/limageio.cpp

HEADERS  += mainwindow.h \
    imageworker.h \
    #source/lcolorlist.h \
    source/toolbox.h \
    source/toolboxitem.h \
    source/workerthread.h \
#    source/limage/limage.h \
    #source/limage/limageqimage.h \
    #source/limage/multicolorimage.h \
    #source/limage/limagefactory.h \
    #source/limage/standardcolorimage.h \
    imageeditor.h \
    dialognewimage.h \
    dialogimport.h \
 #   source/limage/limageio.h

FORMS    += mainwindow.ui \
    dialognewimage.ui \
    dialogimport.ui

RESOURCES += \
    resources.qrc


#LELIB INCLUDES

INCLUDEPATH += $$PWD/../lelib/
DEPENDPATH += $$PWD/../lelib/

win32-g++ {
 win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LeLib/release/release/ -llelib
 else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LeLib/debug/ -llelib
 else:symbian: LIBS += -llelib
 else:unix: LIBS += -L$$OUT_PWD/../LeLib/debug/ -llelib

 win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../LeLib/release/release/liblelib.a
 else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lelib/debug/lelib.lib
 else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../projects/lelib/liblelib.a

}
win32-msvc*{

 win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LeLib/release/ -llelib
 else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LeLib/debug/ -llelib
 else:symbian: LIBS += -llelib
 else:unix: LIBS += -L$$OUT_PWD/../LeLib/debug/ -llelib

 win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LeLib/release/lelib.lib
 else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lelib/debug/lelib.lib
 else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../projects/lelib/liblelib.a

}



#LELIB INCLUDES ENDS

#CONFIG+=USE_LIBTIFF
#DEFINES += USE_LIBTIFF

# TIFF INCLUDES

USE_LIBTIFF {

LIBS += -L$$OUT_PWD/../LeLib/lib/ -llibtiff

INCLUDEPATH += $$PWD/../Nutil/lib/libtiff
INCLUDEPATH += $$PWD/../Nutil/lib
DEPENDPATH += $$PWD/../Nutil/lib/libtiff

}
# TIFF END INCLUDES
