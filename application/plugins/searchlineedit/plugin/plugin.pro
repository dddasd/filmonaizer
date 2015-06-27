CONFIG       += designer plugin release
TARGET       = $$qtLibraryTarget(q4c_searchlineedit)
TEMPLATE     = lib
DESTDIR      = $(QTDIR)/plugins/designer

HEADERS      = customwidget.h \
             customwidgets.h

SOURCES      = customwidgets.cpp

RESOURCES    += icons.qrc

include(../src/searchlineedit.pri)

target.path  = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS     += target
