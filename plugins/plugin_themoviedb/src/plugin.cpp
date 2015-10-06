#include "plugin.h"

void PluginSearchTheMovieDB::init_plug() {
    m_Notifyer = 0;
    clear_map();
}

void PluginSearchTheMovieDB::clear_map() {
    FmapTags.insert("{:adult}","");
    FmapTags.insert("{:budget}","");
    FmapTags.insert("{:homepage}","");
    FmapTags.insert("{:id}","");
    FmapTags.insert("{:imdb_id}","");
    FmapTags.insert("{:original_language}","");
    FmapTags.insert("{:original_title}","");
    FmapTags.insert("{:overview}","");
    FmapTags.insert("{:popularity}","");
    FmapTags.insert("{:release_date}","");
    FmapTags.insert("{:revenue}","");
    FmapTags.insert("{:runtime}","");
    FmapTags.insert("{:status}","");
    FmapTags.insert("{:tagline}","");
    FmapTags.insert("{:title}","");
    FmapTags.insert("{:vote_average}","");
    FmapTags.insert("{:vote_count}","");

    FmapTags.insert("{:actor}","");
    FmapTags.insert("{:director}","");
    FmapTags.insert("{:producer}","");
    FmapTags.insert("{:compositor}","");
    FmapTags.insert("{:scenario}","");
    FmapTags.insert("{:operator}","");

    FmapTags.insert("{:genres}","");
    FmapTags.insert("{:production_companies}","");
    FmapTags.insert("{:production_countries}","");
    FmapTags.insert("{:spoken_languages}","");
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
            query.addQueryItem("append_to_response","credits");
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
    if ((index >= 0) && (index < res_search.count())) {
        Fdir_temp = dir_tmp;
        QFile file(QDir::toNativeSeparators(QString("%1/%2.sm").arg(Fdir_temp).arg(res_search[index].url_f)));
        if (!file.exists(file.fileName())) {
            QString url(QString("http://api.themoviedb.org/3/movie/%1/images?%2").arg(res_search[index].url_f));
            QUrlQuery query;
            query.addQueryItem("api_key",api_key);
            query.addQueryItem("language","ru");
            query.addQueryItem("include_image_language","en,ru,null");
            url = url.arg(query.toString(QUrl::EncodeSpaces));

            http_download *hd_search = new http_download(this,url,2,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();

        } else {
            file.open(QIODevice::ReadOnly);
            pars_small_image(file.readAll());
            file.close();
        }
    }
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
        case 2: {
            if (ret_code.code_r!=0) {
                QList<QString> ll;
                ll << ret_code.error_string;
                emit m_Notifyer->signalSmallImage(1,ll);
            }
            else {
                QRegExp rr ("movie/([^\\D]\\d+)+\\D");
                if (rr.indexIn(ret_code.url,0)!=(-1)) {
                    QFile file(QDir::toNativeSeparators(QString("%1/%2.sm").arg(Fdir_temp).arg(rr.cap(1))));
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.write(ret_code.buf_d);
                        file.close();
                    }
                    pars_small_image(ret_code.buf_d);
                }
                else {
                    QList<QString> ll;
                    ll << "not film sm return";
                    emit m_Notifyer->signalSmallImage(1,ll);
                }
            }
            break;
        }
        case 3: {
            if (ret_code.code_r!=0) {
                //emit m_Notifyer->signalPars(1,ret_code.error_string);
            }
            else {
                for (int i = 0; i < FlistSmallImageFull.count(); i++) {
                    if (FlistSmallImageFull[i].url == ret_code.url) {
                        QPixmap pixmap;
                        pixmap.loadFromData(ret_code.buf_d);
                        pixmap = pixmap.scaledToWidth(default_width_sm_image,Qt::SmoothTransformation);
                        pixmap.save(FlistSmallImageFull[i].path_image);
                        break;
                    }
                }
            }
            break;
        }
        case 4: {
            if (ret_code.code_r!=0) {
                //emit m_Notifyer->signalPars(1,ret_code.error_string);
            }
            else {
                QRegExp rr ("(.*)+/+([^/]*)+.jpg");
                if (rr.indexIn(ret_code.url,0)!=(-1)) {
                    QFile file(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_temp).arg(rr.cap(2))));
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.write(ret_code.buf_d);
                        file.close();
                    }
                    emit m_Notifyer->signalDownloadImage(rr.cap(2),0);
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
    QJsonParseError ir;
    QJsonDocument jDoc = QJsonDocument::fromJson(buf,&ir);

    qDebug() << buf;

    if (ir.error != 0) {
        emit m_Notifyer->signalPars(1,ir.errorString());
        return;
    }

    QList<QString> list_tags_ar;
    list_tags_ar << "{:actor}" << "{:director}" << "{:producer}" << "{:compositor}" << "{:scenario}" << "{:operator}"
                 << "{:genres}" << "{:production_companies}" << "{:production_countries}" << "{:spoken_languages}";

    QJsonObject jOb = jDoc.object();
    QMapIterator<QString,QString> i(FmapTags);
    while (i.hasNext()) {
        i.next();
        if (list_tags_ar.contains(i.key())) continue;
        QRegExp rr("\\{:([^}]*)\\}");
        if (rr.indexIn(i.key())==(-1)) continue;
        FmapTags[i.key()] = jOb[rr.cap(1)].toVariant().toString();
    }

    QJsonArray jAr = jOb["genres"].toArray();
    FmapTags["{:genres}"] = "";
    for (int i = 0; i < jAr.count(); i++) {
        QJsonObject jObTemp = jAr[i].toObject();
        if (!FmapTags["{:genres}"].isEmpty()) FmapTags["{:genres}"] += ",";
        FmapTags["{:genres}"] += jObTemp["name"].toVariant().toString();
    }

    jAr = jOb["production_companies"].toArray();
    FmapTags["{:production_companies}"] = "";
    for (int i = 0; i < jAr.count(); i++) {
        QJsonObject jObTemp = jAr[i].toObject();
        if (!FmapTags["{:production_companies}"].isEmpty()) FmapTags["{:production_companies}"] += ",";
        FmapTags["{:production_companies}"] += jObTemp["name"].toVariant().toString();
    }

    jAr = jOb["production_countries"].toArray();
    FmapTags["{:production_countries}"] = "";
    for (int i = 0; i < jAr.count(); i++) {
        QJsonObject jObTemp = jAr[i].toObject();
        if (!FmapTags["{:production_countries}"].isEmpty()) FmapTags["{:production_countries}"] += ",";
        FmapTags["{:production_countries}"] += jObTemp["name"].toVariant().toString();
    }

    jAr = jOb["spoken_languages"].toArray();
    FmapTags["{:spoken_languages}"] = "";
    for (int i = 0; i < jAr.count(); i++) {
        QJsonObject jObTemp = jAr[i].toObject();
        if (!FmapTags["{:spoken_languages}"].isEmpty()) FmapTags["{:spoken_languages}"] += ",";
        FmapTags["{:spoken_languages}"] += jObTemp["name"].toVariant().toString();
    }

    //credits
    QJsonObject jObCred = jOb["credits"].toObject();
    jAr = jObCred["cast"].toArray();
    FmapTags["{:actor}"] = "";
    for (int i = 0; i < jAr.count(); i++) {
        QJsonObject jObTemp = jAr[i].toObject();
        if (!FmapTags["{:actor}"].isEmpty()) FmapTags["{:actor}"] += ",";
        FmapTags["{:actor}"] += jObTemp["name"].toVariant().toString();
    }

    jAr = jObCred["crew"].toArray();
    //"{:director}" << "{:producer}" << "{:compositor}" << "{:scenario}" << "{:operator}"
    FmapTags["{:director}"] = "";
    FmapTags["{:producer}"] = "";
    FmapTags["{:compositor}"] = "";
    FmapTags["{:scenario}"] = "";
    FmapTags["{:operator}"] = "";
    for (int i = 0; i < jAr.count(); i++) {
        QJsonObject jObTemp = jAr[i].toObject();

        if (jObTemp["job"].toString() == "Director") {
            if (!FmapTags["{:director}"].isEmpty()) FmapTags["{:director}"] += ",";
            FmapTags["{:director}"] += jObTemp["name"].toVariant().toString();
        }

        if (jObTemp["department"].toString() == "Writing") {
            if (!FmapTags["{:scenario}"].isEmpty()) FmapTags["{:scenario}"] += ",";
            FmapTags["{:scenario}"] += jObTemp["name"].toVariant().toString();
        }

        if (jObTemp["job"].toString() == "Director of Photography") {
            if (!FmapTags["{:operator}"].isEmpty()) FmapTags["{:operator}"] += ",";
            FmapTags["{:operator}"] += jObTemp["name"].toVariant().toString();
        }

        if (jObTemp["job"].toString() == "Original Music Composer") {
            if (!FmapTags["{:compositor}"].isEmpty()) FmapTags["{:compositor}"] += ",";
            FmapTags["{:compositor}"] += jObTemp["name"].toVariant().toString();
        }

        if ((jObTemp["job"].toString() == "Producer") || (jObTemp["job"].toString() == "Executive Producer")) {
            if (!FmapTags["{:producer}"].isEmpty()) FmapTags["{:producer}"] += ",";
            FmapTags["{:producer}"] += jObTemp["name"].toVariant().toString();
        }
    }

    emit m_Notifyer->signalPars(0,"");
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

void PluginSearchTheMovieDB::pars_small_image(QByteArray buf) {
    QJsonParseError ir;
    QJsonDocument jDoc = QJsonDocument::fromJson(buf,&ir);

    QList<QString> res_sm;

    qDebug() << buf;

    if (ir.error != 0) {
        res_sm << ir.errorString();
        emit m_Notifyer->signalSmallImage(1,res_sm);
        return;
    }

    QJsonObject jOb = jDoc.object();

    QJsonArray jAr = jOb["posters"].toArray();
    for (int i = 0; i < jAr.count(); i++) {
        sm_image tmp_sm;
        QJsonObject jObTemp = jAr[i].toObject();
        QRegExp rr("/+(.*)+.jpg");
        rr.indexIn(jObTemp["file_path"].toVariant().toString(),0);
        tmp_sm.id = rr.cap(1);
        tmp_sm.url = QString("http://image.tmdb.org/t/p/w185/%1.jpg").arg(tmp_sm.id);
        tmp_sm.path_image = QDir::toNativeSeparators(QString("%1/sm_%2.jpg").arg(Fdir_temp).arg(tmp_sm.id));

        bool b = false;
        for (int i = 0; i < FlistSmallImageFull.count();i++) {
            if (FlistSmallImageFull[i].id == tmp_sm.id) {
                b = true;
                tmp_sm.path_image = FlistSmallImageFull[i].path_image;
            }
        }
        if (!b) FlistSmallImageFull.append(tmp_sm);

        if (!QFile(tmp_sm.path_image).exists()) {
            http_download *hd_search = new http_download(this,tmp_sm.url,3,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();
        }
        res_sm.append(QString("%1").arg(tmp_sm.id));
    }

    emit m_Notifyer->signalSmallImage(0,res_sm);
}

void PluginSearchTheMovieDB::download_image(QString id,QString dir_tmp) {
//http://image.tmdb.org/t/p/original/
    Fdir_temp = dir_tmp;
    if (!QFile(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_temp).arg(id))).exists()) {
        http_download *hd_search = new http_download(this,QString("http://image.tmdb.org/t/p/original/%1.jpg").arg(id),4,Fproxy,Fhost,Fport,Fusername,Fpassword);
        connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
        hd_search->_download();
    } else {
        emit m_Notifyer->signalDownloadImage(id,0);
    }
}

void PluginSearchTheMovieDB::download_all_image(QString dir_tmp) {
    Fdir_temp = dir_tmp;
    for (int i = 0; i < FlistSmallImageFull.count(); i++) {
        if (!QFile(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_temp).arg(FlistSmallImageFull[i].id))).exists()) {
            http_download *hd_search = new http_download(this,QString("http://image.tmdb.org/t/p/original/%1.jpg").arg(FlistSmallImageFull[i].id),4,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();
        } else {
            emit m_Notifyer->signalDownloadImage(FlistSmallImageFull[i].id,0);
        }
    }
}

QString PluginSearchTheMovieDB::getVersion() {
    return version;
}

