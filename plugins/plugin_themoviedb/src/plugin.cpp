#include "plugin.h"

void PluginSearchTheMovieDB::init_plug() {
    m_Notifyer = 0;
    clear_map();
}

void PluginSearchTheMovieDB::clear_map() {
    FmapTags.insert("{:adult}","");
    FmapTags.insert("{:belongs_to_collection}","");
    FmapTags.insert("{:budget}","");
    FmapTags.insert("{:genres}","");
    FmapTags.insert("{:homepage}","");
    FmapTags.insert("{:id}","");
    FmapTags.insert("{:imdb_id}","");
    FmapTags.insert("{:original_language}","");
    FmapTags.insert("{:original_title}","");
    FmapTags.insert("{:overview}","");
    FmapTags.insert("{:popularity}","");
    FmapTags.insert("{:production_companies}","");
    FmapTags.insert("{:production_countries}","");
    FmapTags.insert("{:release_date}","");
    FmapTags.insert("{:revenue}","");
    FmapTags.insert("{:runtime}","");
    FmapTags.insert("{:spoken_languages}","");
    FmapTags.insert("{:status}","");
    FmapTags.insert("{:tagline}","");
    FmapTags.insert("{:title}","");
    FmapTags.insert("{:video}","");
    FmapTags.insert("{:vote_average}","");
    FmapTags.insert("{:vote_count}","");
}

void PluginSearchTheMovieDB::set_proxy(bool proxy, QString host, QString port, QString username, QString password) {
    Fproxy = proxy;
    Fhost = host;
    Fport = port;
    Fusername = username;
    Fpassword = password;
}

void PluginSearchTheMovieDB::result_search_movie(QString str) {
    if (str=="") return;
    QString url;
    Fsearch_link = false;
    res_search.clear();
    QRegExp rr("https://(.*)themoviedb.org/(.*)");
    if (rr.indexIn(str,0)!=(-1)) {
        url = "https://www.themoviedb.org/"+rr.cap(2);
        Fsearch_link = true;
    } else {
        url = QString("http://api.themoviedb.org/3/search/movie?%1");

        QUrlQuery query;
        query.addQueryItem("api_key",api_key);
        query.addQueryItem("language","ru");
        query.addQueryItem("include_adult","true");

        QRegExp rr1("(.*)?([0-9]{4})$");
        if (rr1.indexIn(str,0)!=(-1)) {
            query.addQueryItem("primary_release_year",rr1.cap(2));
            //query.addQueryItem("year",rr1.cap(2));
            query.addQueryItem("query",rr1.cap(1));
        } else {
            query.addQueryItem("query",str);
        }
        url = url.arg(query.toString(QUrl::EncodeSpaces));
    }

    http_download *hd_search = new http_download(this,url,0,Fproxy,Fhost,Fport,Fusername,Fpassword);
    connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
    hd_search->_download();
}

void PluginSearchTheMovieDB::result_pars_movie(int index, QString dir_tmp) {
    if ((index >= 0) && (index < res_search.count())) {
        Fdir_temp = dir_tmp;
        QFile file(QDir::toNativeSeparators(QString("%1/%2.ops").arg(Fdir_temp).arg(res_search[index].url_f)));
        if (!file.exists(file.fileName())) {
            QString url(QString("http://api.themoviedb.org/3/movie/%1?%2").arg(res_search[index].url_f));
            QUrlQuery query;
            query.addQueryItem("api_key",api_key);
            query.addQueryItem("language","ru");
            url = url.arg(query.toString(QUrl::EncodeSpaces));

            http_download *hd_search = new http_download(this,url,1,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();

        } else {
            file.open(QIODevice::ReadOnly);
            pars_film_result(file.readAll());
            file.close();
        }
    }
}

void PluginSearchTheMovieDB::result_search_small_image(int index, QString dir_tmp) {

}

void PluginSearchTheMovieDB::fin_d(result_url ret_code) {
    switch (ret_code.id_msg) {
        case 0: {
            if (Fsearch_link) {
                if (ret_code.code_r!=0) {
                    QList<QString> s;
                    emit m_Notifyer->signalSearch(s,1,ret_code.error_string);
                } else {
                    pars_film_result(ret_code.buf_d);
                }
            } else {
                if (ret_code.code_r!=0) {
                    QList<QString> s;
                    emit m_Notifyer->signalSearch(s,1,ret_code.error_string);
                } else { //Всё ок
                    pars_search_result(ret_code.buf_d);
                }
            }
            break;
        }
        case 1: {
            if (ret_code.code_r!=0) {
                emit m_Notifyer->signalPars(1,ret_code.error_string);
            }
            else {
                QRegExp rr ("movie/([^\\D]\\d+)+\\D");
                if (rr.indexIn(ret_code.url,0)!=(-1)) {
                    QFile file(QDir::toNativeSeparators(QString("%1/%2.ops").arg(Fdir_temp).arg(rr.cap(1))));
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.write(ret_code.buf_d);
                        file.close();
                    }
                    pars_film_result(ret_code.buf_d);
                }
                else {
                    emit m_Notifyer->signalPars(1,"not film ops return");
                }
            }
            break;
        }
    }
}

void PluginSearchTheMovieDB::pars_search_result(QByteArray buf) {
    QList<QString> res_signal;
    QJsonParseError ir;
    QJsonDocument jDoc = QJsonDocument::fromJson(buf,&ir);

    qDebug() << buf;

    if (ir.error != 0) {
        emit m_Notifyer->signalSearch(res_signal,1,ir.errorString());
        return;
    }
    QJsonObject jOb = jDoc.object();
    QJsonArray jAr = jOb["results"].toArray();
    for (int i = 0; i < jAr.count(); i++) {
        QJsonObject jObTemp = jAr[i].toObject();
        result_search poisk_s;
        poisk_s.name_eng = jObTemp["original_title"].toString();
        poisk_s.name_ru = jObTemp["title"].toString();
        QString gg = jObTemp["release_date"].toString();
        gg.remove(4,gg.length()-4);
        poisk_s.year_f = gg;
        poisk_s.url_f = QString("%1").arg(jObTemp["id"].toInt());

        res_search << poisk_s;
    }

    if (res_search.isEmpty()) emit m_Notifyer->signalSearch(res_signal,0,"");

    for (int i=0;i<res_search.size();i++) {
        if (!res_search[i].name_eng.isEmpty() & !res_search[i].name_ru.isEmpty())
            res_signal.append(res_search[i].name_eng+" \\ "+res_search[i].name_ru+ " ("+res_search[i].year_f+")");
        else {
            if (res_search[i].name_eng.isEmpty())
                res_signal.append(res_search[i].name_ru+ " ("+res_search[i].year_f+")");
            else
                res_signal.append(res_search[i].name_eng+ " ("+res_search[i].year_f+")");
        }
    }

    emit m_Notifyer->signalSearch(res_signal,0,"");
}

void PluginSearchTheMovieDB::pars_film_result(QByteArray buf) {

}

QString PluginSearchTheMovieDB::html_decode(QString stroka) {

}

QList<QString> PluginSearchTheMovieDB::listTags() {
    QList<QString> ret;

    QMapIterator<QString,QString> i(FmapTags);
    i.toFront();
    while (i.hasNext()) {
        i.next();
        ret.append(i.key());
    }
    return ret;
}

QString PluginSearchTheMovieDB::result_tags(QString tt) {
    QMap<QString,QString>::const_iterator i = FmapTags.find(tt);
    if (i.key() == tt) return i.value();
    else return "";
}

void PluginSearchTheMovieDB::pars_small_image(QString code_film, QByteArray buf) {

}

void PluginSearchTheMovieDB::pars_image(QByteArray buf) {

}

void PluginSearchTheMovieDB::download_image(QString id,QString dir_tmp) {

}

void PluginSearchTheMovieDB::download_all_image(QString dir_tmp) {

}
