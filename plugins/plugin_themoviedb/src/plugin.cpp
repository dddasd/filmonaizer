#include "plugin.h"

void PluginSearchTheMovieDB::init_plug() {
    m_Notifyer = 0;
    clear_map();
}

void PluginSearchTheMovieDB::clear_map() {
    FmapTags.insert("{:actor}","");
    FmapTags.insert("{:budget}","");
    FmapTags.insert("{:code_film_kp}","");
    FmapTags.insert("{:compositor}","");
    FmapTags.insert("{:country}","");
    FmapTags.insert("{:desc}","");
    FmapTags.insert("{:director}","");
    FmapTags.insert("{:dublicate}","");
    FmapTags.insert("{:genre}","");
    FmapTags.insert("{:operator}","");
    FmapTags.insert("{:premier_ru}","");
    FmapTags.insert("{:premier_world}","");
    FmapTags.insert("{:producer}","");
    FmapTags.insert("{:rating_imbd}","");
    FmapTags.insert("{:rating_kp}","");
    FmapTags.insert("{:rating_MPAA}","");
    FmapTags.insert("{:release_DVD}","");
    FmapTags.insert("{:scenario}","");
    FmapTags.insert("{:slogan}","");
    FmapTags.insert("{:time}","");
    FmapTags.insert("{:title_eng}","");
    FmapTags.insert("{:title_eng_clear}","");
    FmapTags.insert("{:title_eng_ru}","");
    FmapTags.insert("{:title_ru}","");
    FmapTags.insert("{:trailer}","");
    FmapTags.insert("{:url_kp}","");
    FmapTags.insert("{:year}","");
}

void PluginSearchTheMovieDB::set_proxy(bool proxy, QString host, QString port, QString username, QString password) {
    Fproxy = proxy;
    Fhost = host;
    Fport = port;
    Fusername = username;
    Fpassword = password;
}

void PluginSearchTheMovieDB::result_search_movie(QString str) {

}

void PluginSearchTheMovieDB::result_pars_movie(int index, QString dir_tmp) {

}

void PluginSearchTheMovieDB::result_search_small_image(int index, QString dir_tmp) {

}

QString PluginSearchTheMovieDB::chartoascii(QString ss) {

}


void PluginSearchTheMovieDB::pars_search_result(QByteArray buf, QString year_f) {

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
