TEMPLATE        = lib
CONFIG         += plugin
QT             += core network
INCLUDEPATH    += ../../application/src
HEADERS         = src/plugin.h \
                ../../application/src/http_download.h
SOURCES         = src/plugin.cpp \
                ../../application/src/http_download.cpp
TARGET          = plugin_themoviedb
DESTDIR         = ../../../../bin/plugins

# install
target.path = ../../../../bin/plugins
INSTALLS += target
