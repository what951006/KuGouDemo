#-------------------------------------------------
#
# Project created by QtCreator 2016-06-14T12:16:34
#
#-------------------------------------------------

QT       += core gui sql network multimedia ##webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11
INCLUDEPATH+=$$PWD/threadFiles \
             $$PWD/dataBase \
             $$PWD/deskTopLrc

INCLUDEPATH +=$$PWD trayIconMenu
include(trayIconMenu/trayiconmenu.pri)

INCLUDEPATH +=$$PWD tableWidgetFiles
include(tableWidgetFiles/tablewidgetplaylist.pri)

INCLUDEPATH +=$$PWD PlayCore
include(PlayCore/player.pri)

INCLUDEPATH +=$$PWD netWork
include(netWork/network.pri)

INCLUDEPATH +=$$PWD topWidget
include(topWidget/topwidget.pri)

INCLUDEPATH +=$$PWD middleWidget
include(middleWidget/middlewidget.pri)

INCLUDEPATH +=$$PWD bottomWidget
include(bottomWidget/bottomwidget.pri)

INCLUDEPATH +=$$PWD otherWidget
include(otherWidget/otherWidget.pri)

INCLUDEPATH +=$$PWD absFiles
include(absFiles/absFiles.pri)

INCLUDEPATH +=$$PWD mainWindows
include(mainWindows/mainWindows.pri)

SOURCES +=$$PWD/singleApplication/qsingleapplication.cpp \
         $$PWD/dataBase/myDataBase.cpp \
         $$PWD/deskTopLrc/deskTopLrcWidget.cpp \


HEADERS +=$$PWD/singleApplication/qsingleapplication.h \
          $$PWD/dataBase/myDataBase.h \
          $$PWD/deskTopLrc/deskTopLrcWidget.h

RESOURCES += \
    res_topwidget.qrc \
    res_middlewidget.qrc \
    res_main.qrc \
    res_traymenu.qrc\
    res_bottomwidget.qrc

RC_FILE = image/main/kwgou.rc

