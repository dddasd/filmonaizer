QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = ../../bin

TEMPLATE = app
RESOURCES += icons/filmonaizer.qrc
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/http_download.cpp \
    src/previewdesc.cpp \
    src/aboutdialog.cpp \
    src/dialog_getupdates.cpp \
    src/dialog_version.cpp \
    src/set_proxy.cpp
HEADERS += src/mainwindow.h \
    src/qinterfacepluginsearch.h \
    src/qinterfacepluginmovie.h \
    src/http_download.h \
    src/previewdesc.h \
    src/aboutdialog.h \
    src/dialog_getupdates.h \
    src/dialog_version.h \
    src/set_proxy.h
FORMS += src/mainwindow.ui \
    src/aboutdialog.ui \
    src/set_proxy.ui \
    src/preview_desc.ui
TRANSLATIONS += translate/filmonaizer_ru_RU.ts
VERSION = 0.3.26
include(plugins/searchlineedit/src/searchlineedit.pri)
