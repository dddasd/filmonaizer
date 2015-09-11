TEMPLATE        = lib
CONFIG         += plugin
QT             += core
INCLUDEPATH    += ../../application/src
HEADERS         = src/plugin.h \
                src/MediaInfoDLL.h
SOURCES         = src/plugin.cpp
TARGET          = plugin_mediainfo
DESTDIR         = ../../../../bin/plugins

# install
target.path = ../../../../bin/plugins
INSTALLS += target
