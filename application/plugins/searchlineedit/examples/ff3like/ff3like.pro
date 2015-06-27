TEMPLATE     = app

TARGET       = ff3like

INCLUDEPATH  += .

HEADERS      = dialog.h

SOURCES      = dialog.cpp \
             main.cpp

RESOURCES    += icons.qrc

include(../../src/searchlineedit.pri)
