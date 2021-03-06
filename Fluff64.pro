#-------------------------------------------------
#
# Project created by QtCreator 2018-02-03T17:17:06
#
#-------------------------------------------------

QT       += core gui
QT += widgets


TARGET = Fluff64
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make yopur code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -fopenmp -std=c++11

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
    source/toolbox.cpp \
    source/toolboxitem.cpp \
    source/workerthread.cpp \
    imageeditor.cpp \
    dialognewimage.cpp \
    dialogimport.cpp \
    source/PmmEdit/highlighter.cpp\
    source/PmmEdit/codeeditor.cpp\
# PMM
    ..\Pmm\source\data_pmm.cpp \
    ..\Pmm\source\token.cpp \
    ..\Pmm\source\lexer.cpp \
    ..\Pmm\source\syntax.cpp \
    ..\Pmm\source\ast\ast.cpp \
    ..\Pmm\source\interpreter.cpp \
    ..\Pmm\source\symboltable.cpp \
    ..\Pmm\source\errorhandler.cpp \
    ..\Pmm\source\pvar.cpp \
    ..\Pmm\source\parser.cpp \
    ..\Pmm\source/misc/sidfile.cpp \
    ..\Pmm\source/ast/node.cpp \
    ..\Pmm\source/ast/nodebinop.cpp \
    ..\Pmm\source/ast/nodenumber.cpp \
    ..\Pmm\source/ast/nodestring.cpp \
    ..\Pmm\source/ast/nodeunaryop.cpp \
    ..\Pmm\source/ast/nodecompound.cpp \
    ..\Pmm\source/ast/nodevar.cpp \
    ..\Pmm\source/ast/nodevararray.cpp \
    ..\Pmm\source/ast/nodeassign.cpp \
    ..\Pmm\source/ast/nodevardecl.cpp \
    ..\Pmm\source/ast/nodeblock.cpp \
    ..\Pmm\source/ast/nodeprogram.cpp \
    ..\Pmm\source/ast/nodevartype.cpp \
    ..\Pmm\source/ast/nodeproceduredecl.cpp \
    ..\Pmm\source/ast/nodeprocedure.cpp \
    ..\Pmm\source/ast/nodeconditional.cpp \
    ..\Pmm\source/ast/nodeasm.cpp \
    ..\Pmm\source/ast/nodewhileloop.cpp \
    ..\Pmm\source/ast/nodeforloop.cpp \
    ..\Pmm\source/ast/nodebuiltinmethod.cpp \
    ..\Pmm\source/assembler/assembler.cpp\
    ..\Pmm\source/assembler/mos6502.cpp\
    ..\Pmm\source/assembler/asmpascal.cpp \
    ../Pmm/source/ast/nodebinaryclause.cpp \
    source/trsedocuments/trsedocument.cpp \
    source/trsedocuments/formraseditor.cpp \
    source/trsedocuments/formimageeditor.cpp \
    source/dialogtrsesettings.cpp \
    source/dialogmessage.cpp \
    source/messages.cpp \
    source/dialogabout.cpp \
    source/dialoghelp.cpp \
    source/dialogexport.cpp \
    source/dialogmemoryanalyze.cpp


HEADERS  += mainwindow.h \
    imageworker.h \
    source/toolbox.h \
    source/toolboxitem.h \
    source/workerthread.h \
    imageeditor.h \
    dialognewimage.h \
    dialogimport.h \
    source/PmmEdit/highlighter.h \
# PMM
    ..\Pmm\source\data_pmm.h \
    ..\Pmm\source\token.h \
    ..\Pmm\source\lexer.h \
    ..\Pmm\source\syntax.h \
    ..\Pmm\source\ast\ast.h \
    ..\Pmm\source\interpreter.h \
    ..\Pmm\source\symboltable.h \
    ..\Pmm\source\errorhandler.h \
    ..\Pmm\source\pvar.h \
    ..\Pmm\source\parser.h \
    ..\Pmm\source/misc/sidfile.h \
    ..\Pmm\source/ast/node.h \
    ..\Pmm\source/ast/nodebinop.h \
    ..\Pmm\source/ast/nodenumber.h \
    ..\Pmm\source/ast/nodestring.h \
    ..\Pmm\source/ast/nodeunaryop.h \
    ..\Pmm\source/ast/nodecompound.h \
    ..\Pmm\source/ast/nodevar.h \
    ..\Pmm\source/ast/nodevararray.h \
    ..\Pmm\source/ast/nodeassign.h \
    ..\Pmm\source/ast/nodevardecl.h \
    ..\Pmm\source/ast/nodeblock.h \
    ..\Pmm\source/ast/nodeasm.h \
    ..\Pmm\source/ast/nodeprogram.h \
    ..\Pmm\source/ast/nodevartype.h \
    ..\Pmm\source/ast/nodeproceduredecl.h \
    ..\Pmm\source/ast/nodeprocedure.h \
    ..\Pmm\source/ast/nodeconditional.h \
    ..\Pmm\source/ast/nodewhileloop.h \
    ..\Pmm\source/ast/nodeforloop.h \
    ..\Pmm\source/ast/nodebuiltinmethod.h\
    ..\Pmm\source/assembler/assembler.h\
    ..\Pmm\source/assembler/mos6502.h\
    ..\Pmm\source/assembler/asmpascal.h \
    ../Pmm/source/ast/nodebinaryclause.h \
    source/PmmEdit/codeeditor.h \
    source/trsedocuments/formraseditor.h \
    source/trsedocuments/trsedocument.h \
    source/trsedocuments/formimageeditor.h \
    source/dialogtrsesettings.h \
    source/dialogmessage.h \
    source/messages.h \
    source/dialogabout.h \
    source/dialoghelp.h \
    source/dialogexport.h \
    source/dialogmemoryanalyze.h

FORMS    += mainwindow.ui \
    dialognewimage.ui \
    dialogimport.ui \
    source/trsedocuments/formraseditor.ui \
    source/trsedocuments/formimageeditor.ui \
    source/dialogtrsesettings.ui \
    source/dialogmessage.ui \
    source/dialogabout.ui \
    source/dialoghelp.ui \
    source/dialogexport.ui \
    source/dialogmemoryanalyze.ui

RESOURCES += \
    resources.qrc


#LELIB INCLUDES

INCLUDEPATH += $$PWD/../LeLib/
DEPENDPATH += $$PWD/../LeLib/


INCLUDEPATH += $$PWD/../Pmm/
DEPENDPATH += $$PWD/../Pmm/


win32-g++ {
 win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LeLib/release/release/ -llelib
 else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LeLib/debug/ -llelib
 else:symbian: LIBS += -llelib
 else:unix: LIBS += -L$$OUT_PWD/../LeLib/debug/ -llelib

 win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../LeLib/release/release/liblelib.a
 else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lelib/debug/deblelib.lib
 else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../projects/lelib/liblelib.a

}
win32-msvc*{

 win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LeLib/release/release/ -llelib
 else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LeLib/debug/debug/ -llelib
 else:symbian: LIBS += -llelib
 else:unix: LIBS += -L$$OUT_PWD/../LeLib/debug/ -llelib

 win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LeLib/release/release/lelib.lib
 else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lelib/debug/debug/lelib.lib
 else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../projects/lelib/liblelib.a

}
linux-g++ {

    LIBS += -L$$PWD/../LeLib/release/ -lLeLib
}

    LIBS += -L$$PWD/../LeLib/release/ -lLeLib -fopenmp

#LELIB INCLUDES ENDS

#CONFIG+=USE_LIBTIFF
#DEFINES += USE_LIBTIFF

# TIFF INCLUDES

USE_LIBTIFF {

LIBS += -L$$OUT_PWD/../LeLib/lib/ -llibtiff

INCLUDEPATH += $$PWD/../LeLib/lib/libtiff
INCLUDEPATH += $$PWD/../LeLib/lib
DEPENDPATH += $$PWD/../LeLib/lib/libtiff

}
# TIFF END INCLUDES

DISTFILES += \
    resources/text/about.txt \
    resources/text/Documentation.txt
