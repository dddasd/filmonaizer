#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QTextCodec>
#include <QRegExp>
#include <QList>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "qinterfacepluginmovie.h"
#include "MediaInfoDLL.h"

#define version "1.0.0"

class NotifyerMovie: public INotifyerMovie {
    Q_OBJECT
public:
    virtual QObject *instance() { return this; }

signals:
    void signalPars(int,QString);
};

class PluginMovieMediaInfo: public QObject, QInterfacePluginMovie {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "FilmonaizerMovie")
    Q_INTERFACES(QInterfacePluginMovie)

public:
    void init_plug() Q_DECL_OVERRIDE;

    void result_movie(QString) Q_DECL_OVERRIDE; //Поиск фильма, аргумент - строка поиска

    QList<QString> listTags() Q_DECL_OVERRIDE; //Список тегов

    QString result_tags(QString) Q_DECL_OVERRIDE; //Возврат значение тега

    QString getVersion() Q_DECL_OVERRIDE; //Версия плагина

    INotifyerMovie* notifyer() {
        if (!m_Notifyer) m_Notifyer = new NotifyerMovie();
        return m_Notifyer;
    }
private:
    NotifyerMovie *m_Notifyer;

    QMap<QString,QString> FmapTags;

    void clear_map();
};

#endif
