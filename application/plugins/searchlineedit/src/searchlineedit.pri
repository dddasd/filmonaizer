INCLUDEPATH  += $$PWD
DEPENDPATH   += $$PWD

HEADERS      += $$PWD/global.h
HEADERS      += $$PWD/animatedclearbutton.h \
             $$PWD/lineedit.h \
             $$PWD/lineedit_p.h \
             $$PWD/proxystyle_p.h \
             $$PWD/searchlineedit.h \
             $$PWD/simplebutton.h

SOURCES      += $$PWD/animatedclearbutton.cpp \
             $$PWD/lineedit.cpp \
             $$PWD/searchlineedit.cpp \
             $$PWD/simplebutton.cpp

RESOURCES    += $$PWD/sle_icons.qrc

win32:contains(TEMPLATE, lib):contains(CONFIG, shared) {
#    DEFINES += Q4CW_SLE_DLL
}
