TEMPLATE        = lib
CONFIG         += plugin
QT             += core network
INCLUDEPATH    += ../filmonaizer/src
HEADERS         = src/plugin.h \
                ../filmonaizer/src/http_download.h
SOURCES         = src/plugin.cpp \
                ../filmonaizer/src/http_download.cpp
TARGET          = plugin_kp
DESTDIR         = ../../bin/plugins

# install
target.path = ../../bin/plugins
INSTALLS += target
