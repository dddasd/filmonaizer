#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QTextCodec>
#include <QRegExp>
#include <QList>
#include <QDir>
#include <QFile>
#include <QPixmap>

#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#include "qinterfacepluginsearch.h"
#include "http_download.h"

#define default_width_sm_image 170
#define api_key "834bec6fc86837ca086473e3c29556e9"

class NotifyerSearch: public INotifyerSearch {
    Q_OBJECT
public:
    virtual QObject *instance() { return this; }

signals:
    void signalSearch(QList<QString>,int,QString);
    void signalPars(int,QString);
    void signalSmallImage(int,QList<QString>);
    void signalDownloadImage(QString,int);
};

class PluginSearchTheMovieDB: public QObject, QInterfacePluginSearch {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "FilmonaizerSearch")
    Q_INTERFACES(QInterfacePluginSearch)

public:
    void init_plug() Q_DECL_OVERRIDE;
    void set_proxy(bool proxy = false, QString host = "", QString port = "", QString username = "", QString password = "") Q_DECL_OVERRIDE;

    void result_search_movie(QString) Q_DECL_OVERRIDE; //Поиск фильма, аргумент - строка поиска
    void result_pars_movie(int,QString) Q_DECL_OVERRIDE; //Загрузка фильма, и возвращение результата в шаблоне
    void result_search_small_image(int,QString) Q_DECL_OVERRIDE; //Загрузка мелких картинок
    void download_image(QString,QString) Q_DECL_OVERRIDE; //Загрузить картинку по её ID
    void download_all_image(QString) Q_DECL_OVERRIDE; //Загрузить все картинки (которые были найдены в мелких)

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
        QString id;
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

    //Папка с темп директорией
    QString Fdir_temp;

    bool Fsearch_link;

    QList<result_search> res_search;

    QMap<QString,QString> FmapTags;

    QList<sm_image> FlistSmallImageFull;

    void pars_search_result(QByteArray);
    QString html_decode(QString);
    void pars_film_result(QByteArray buf);
    void clear_map();
    void pars_small_image(QString code_film, QByteArray buf);
    void pars_image(QByteArray buf);
private slots:
    void fin_d(result_url);
};

#endif
