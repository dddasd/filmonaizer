#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QTextCodec>
#include <QRegExp>
#include <QList>
#include <QDir>
#include <QFile>

#include "qinterfacepluginsearch.h"
#include "http_download.h"

class NotifyerSearch: public INotifyerSearch {
    Q_OBJECT
public:
    virtual QObject *instance() { return this; }

signals:
    void signalSearch(QList<QString>,int,QString);
    void signalPars(int,QString);
    void signalSmallImage(int,QList<QString>);
};

class PluginSearchKP: public QObject, QInterfacePluginSearch {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "FilmonaizerSearch")
    Q_INTERFACES(QInterfacePluginSearch)

public:
    void init_plug() Q_DECL_OVERRIDE;
    void set_proxy(bool proxy = false, QString host = "", QString port = "", QString username = "", QString password = "") Q_DECL_OVERRIDE;

    void result_search_movie(QString) Q_DECL_OVERRIDE; //Поиск фильма, аргумент - строка поиска
    void result_pars_movie(int,QString) Q_DECL_OVERRIDE; //Загрузка фильма, и возвращение результата в шаблоне
    void result_search_small_image(int,QString) Q_DECL_OVERRIDE; //Загрузка мелких картинок

    QList<QString> listTags() Q_DECL_OVERRIDE; //Список тегов

    QString result_tags(QString) Q_DECL_OVERRIDE; //Возврат значение тега

    INotifyerSearch* notifyer() {
        if (!m_Notifyer) m_Notifyer = new NotifyerSearch();
        return m_Notifyer;
    }
private:
    struct result_search {
        QString name_ru;
        QString name_eng;
        QString year_f;
        QString url_f;
    };

    struct sort_year {
        int index;
        int year;
    };

    struct sm_image {
        int id;
        QString url;
        QString path_image;
    };

    NotifyerSearch *m_Notifyer;

    //для прокси
    bool Fproxy;
    QString Fhost;
    QString Fport;
    QString Fusername;
    QString Fpassword;

    //Номер команды
    //int Fnum_com;

    //Папка с темп директорией
    QString Fdir_temp;

    //
    QString Fyear_search;

    bool Fsearch_link;

    QList<result_search> res_search;

    QMap<QString,QString> FmapTags;

    QList<sm_image> FlistSmallImageFull;

    QString chartoascii(QString);
    void pars_search_result(QByteArray,QString);
    QString html_decode(QString);
    void pars_film_result(QByteArray buf);
    void clear_map();
    void pars_small_image(QByteArray buf);
    void pars_image(QByteArray buf);
private slots:
    void fin_d(result_url);
};

#endif
