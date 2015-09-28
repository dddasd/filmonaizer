#include "plugin.h"

void PluginSearchKP::init_plug() {
    m_Notifyer = 0;
    clear_map();
}

void PluginSearchKP::clear_map() {
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

void PluginSearchKP::set_proxy(bool proxy, QString host, QString port, QString username, QString password) {
    Fproxy = proxy;
    Fhost = host;
    Fport = port;
    Fusername = username;
    Fpassword = password;
}

void PluginSearchKP::result_search_movie(QString str) {
    if (str=="") return;
    QString url;
    Fyear_search = "";
    Fsearch_link = false;
    res_search.clear();
    QRegExp rr("http://(.*)kinopoisk.ru/(.*)");
    if (rr.indexIn(str,0)!=(-1)) {
        url = "http://www.kinopoisk.ru/"+rr.cap(2);
        Fsearch_link = true;
    } else {
        QRegExp rr1("(.*)?([0-9]{4})$");
        if (rr1.indexIn(str,0)!=(-1)) {
            Fyear_search = rr1.cap(2);
            url = "http://www.kinopoisk.ru/index.php?kp_query="+chartoascii(rr1.cap(1));
        } else {
            url = "http://www.kinopoisk.ru/index.php?kp_query="+chartoascii(str);
        }
    }
    //Fnum_com = 0;

    http_download *hd_search = new http_download(this,url,0,Fproxy,Fhost,Fport,Fusername,Fpassword);
    connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
    hd_search->_download();
}

void PluginSearchKP::result_pars_movie(int index, QString dir_tmp) {
    if ((index >= 0) && (index < res_search.count())) {
        Fdir_temp = dir_tmp;
        QFile file(QDir::toNativeSeparators(QString("%1/%2.ops").arg(Fdir_temp).arg(res_search[index].url_f)));
        if (!file.exists(file.fileName())) {
            //Fnum_com = 1;
            http_download *hd_search = new http_download(this,"http://www.kinopoisk.ru/film/"+res_search[index].url_f+"/",1,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();

        } else {
            file.open(QIODevice::ReadOnly);
            pars_film_result(file.readAll());
            file.close();
        }
    }
}

void PluginSearchKP::result_search_small_image(int index, QString dir_tmp) {
    if ((index >= 0) && (index < res_search.count())) {
        //FlistSmallImageFull.clear();
        Fdir_temp = dir_tmp;
        QFile file(QDir::toNativeSeparators(QString("%1/%2.sm").arg(Fdir_temp).arg(res_search[index].url_f)));
        if (!file.exists(file.fileName())) {
            //Fnum_com = 2;
            http_download *hd_search = new http_download(this,"http://www.kinopoisk.ru/film/"+res_search[index].url_f+"/covers/",2,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();

        } else {
            file.open(QIODevice::ReadOnly);
            pars_small_image(res_search[index].url_f,file.readAll());
            file.close();
        }
    }
}

QString PluginSearchKP::chartoascii(QString ss) {
    QTextCodec *c;
    c = QTextCodec::codecForName("cp1251");
    QByteArray buf_in(c->fromUnicode(QString::fromLocal8Bit(ss.toLocal8Bit())));
    buf_in = buf_in.toPercentEncoding();

    qDebug() << QString(buf_in);
    return QString(buf_in);
}

void PluginSearchKP::fin_d(result_url ret_code) {
    switch (ret_code.id_msg) {
        case 0: {
            if (Fsearch_link) {
                if (ret_code.code_r!=0) {
                    QList<QString> s;
                    emit m_Notifyer->signalSearch(s,1,ret_code.error_string);
                } else {
                    pars_film_result(ret_code.buf_d);
                    //QList<QString> s;
                    //emit m_Notifyer->signalSearch(s,2,ret_code.error_string);
                }
            } else {
                if (ret_code.code_r!=0) {
                    if (ret_code.code_r==302) { //Редирект
                        QString url;
                        QRegExp rr("http://(.*)kinopoisk.ru/(.*)");
                        if (rr.indexIn(ret_code.url_redirect,0)!=(-1)) {
                            url = ret_code.url_redirect;
                            //Fnum_com = 0;
                        }
                        else {
                            url = "http://www.kinopoisk.ru"+ret_code.url_redirect;
                            url.replace("sr/1/","");
                            //Fnum_com = 0;
                            Fsearch_link = true;
                        }

                        http_download *hd_search = new http_download(this,url,0,Fproxy,Fhost,Fport,Fusername,Fpassword);
                        connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
                        hd_search->_download();
                    } else { //Неудача
                        QList<QString> s;
                        emit m_Notifyer->signalSearch(s,1,ret_code.error_string);
                    }
                } else { //Всё ок
                    pars_search_result(ret_code.buf_d,Fyear_search);
                }
            }
            break;
        }
        case 1: {
            if (ret_code.code_r!=0) {
                emit m_Notifyer->signalPars(1,ret_code.error_string);
            }
            else {
                QRegExp rr ("film/([^\\D]\\d+)+\\D");
                if (rr.indexIn(ret_code.url,0)!=(-1)) {
                    QFile file(QDir::toNativeSeparators(QString("%1/%2.ops").arg(Fdir_temp).arg(rr.cap(1))));
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.write(ret_code.buf_d);
                        file.close();
                    }
                    pars_film_result(ret_code.buf_d);
                    //emit m_Notifyer->signalPars(0,"");
                }
                else {
                    emit m_Notifyer->signalPars(1,"not film ops return");
                }
            }
            break;
        }
        case 2: {
            QRegExp rr ("film/([^\\D]\\d+)+\\D");
            if (rr.indexIn(ret_code.url,0)!=(-1)) {
                QFile file(QDir::toNativeSeparators(QString("%1/%2.sm").arg(Fdir_temp).arg(rr.cap(1))));
                QByteArray bb;
                if (ret_code.code_r != 0) {
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.close();
                    }
                } else {
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.write(ret_code.buf_d);
                        file.close();
                    }
                    bb.append(ret_code.buf_d);
                }

                pars_small_image(rr.cap(1),bb);
            }
            else {
                QList<QString> ss;
                ss.append(QString(tr("small image not search, not regexp")));
                emit m_Notifyer->signalSmallImage(1,ss);
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
                pars_image(ret_code.buf_d);
            }
            break;
        }
        case 5: {
            if (ret_code.code_r!=0) {
                //emit m_Notifyer->signalPars(1,ret_code.error_string);
            }
            else {
                QRegExp rr ("(\\d+).jpg");
                if (rr.indexIn(ret_code.url,0)!=(-1)) {
                    QFile file(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_temp).arg(rr.cap(1))));
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.write(ret_code.buf_d);
                        file.close();
                    }
                    emit m_Notifyer->signalDownloadImage(rr.cap(1),0);
                }
            }
            break;
        }
        case 6: {
            if (ret_code.code_r!=0) {
                //emit m_Notifyer->signalPars(1,ret_code.error_string);
            }
            else {
                QRegExp rr ("(\\d+).jpg");
                if (rr.indexIn(ret_code.url,0)!=(-1)) {
                    QFile file(QDir::toNativeSeparators(QString("%1/main_%2.jpg").arg(Fdir_temp).arg(rr.cap(1))));
                    if (!file.exists(file.fileName())) {
                        file.open(QIODevice::WriteOnly);
                        file.write(ret_code.buf_d);
                        file.close();
                    }
                    QPixmap pixmap;
                    pixmap.loadFromData(ret_code.buf_d);
                    pixmap = pixmap.scaledToWidth(default_width_sm_image,Qt::SmoothTransformation);
                    pixmap.save(QDir::toNativeSeparators(QString("%1/sm_main_%2.jpg").arg(Fdir_temp).arg(rr.cap(1))));
                    emit m_Notifyer->signalDownloadImage(QString("main_%1").arg(rr.cap(1)),0);
                }
            }
            break;
        }
    }
}

void PluginSearchKP::pars_search_result(QByteArray buf, QString year_f) {
    QList<QString> res_signal;

    QList<result_search> res_s;

    QTextCodec *c;
    c = QTextCodec::codecForHtml(buf);
    QString buf_r(c->toUnicode(buf));

    c = QTextCodec::codecForName("cp1251");

    QRegExp reg_s_f("<div class=\"info\">+[^h]+href=\"+([^\"]*)+\">+([^<]*)+[^r]+r\">+([^<]*)+[^y]+y\">+(([^<]*)+,)");
    int pos = reg_s_f.indexIn(buf_r,0);
    while (pos!=(-1)) {
        if (reg_s_f.cap(1).indexOf("/name/")==(-1)) {
            result_search poisk_s;
            QRegExp tempReg("film/+([^/]*)");
            if (tempReg.indexIn(reg_s_f.cap(1),0) != (-1)) poisk_s.url_f=tempReg.cap(1);
            else poisk_s.url_f=reg_s_f.cap(1);
            poisk_s.name_ru=html_decode(reg_s_f.cap(2));
            poisk_s.year_f=reg_s_f.cap(3);
            poisk_s.name_eng=html_decode(reg_s_f.cap(5));
            QRegExp nm_eng(",+([^,]*)$");
            if (nm_eng.indexIn(poisk_s.name_eng)!=(-1)) {
                poisk_s.name_eng.replace(nm_eng.cap(0),"");
            }
            else {
                poisk_s.name_eng="";
            }
            QRegExp eng_reg("^\\.\\.\\.\\s(.*)");
            if (eng_reg.indexIn(poisk_s.name_eng,0)!=(-1))
                poisk_s.name_eng=eng_reg.cap(1);
            res_s << poisk_s;
        }

        pos = reg_s_f.indexIn(buf_r,pos+1);
    }

    bool ok;
    int year_i = year_f.toInt(&ok);
    if (!ok) {
        for (int i=0;i<res_s.size();i++) {
            res_search << res_s[i];
            if (!res_search[i].name_eng.isEmpty() & !res_search[i].name_ru.isEmpty())
                res_signal.append(res_search[i].name_eng+" \\ "+res_search[i].name_ru+ " ("+res_search[i].year_f+")");
            else {
                if (res_search[i].name_eng.isEmpty())
                    res_signal.append(res_search[i].name_ru+ " ("+res_search[i].year_f+")");
                else
                    res_signal.append(res_search[i].name_eng+ " ("+res_search[i].year_f+")");
            }
        }
    }
    else {
        if (!res_s.isEmpty()) {
            QList<sort_year> _sort;
            QList<int> year_perv_sort;
            for (int i=0;i<res_s.size();i++) {
                sort_year ss;
                ss.index = i;
                ss.year = abs(year_i - res_s[i].year_f.toInt());
                _sort << ss;
                year_perv_sort << abs(year_i - res_s[i].year_f.toInt());
            }

            qSort(year_perv_sort.begin(),year_perv_sort.end());
            for (int i=0;i<year_perv_sort.size();i++) {
                for (int j=0;j<_sort.size();j++) {
                    if (year_perv_sort[i]==_sort[j].year) {
                        res_search << res_s[_sort[j].index];
                        break;
                    }
                }
            }

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
        }
    }

    emit m_Notifyer->signalSearch(res_signal,0,"");
}

void PluginSearchKP::pars_film_result(QByteArray buf) {
    clear_map();

    QTextCodec *c;
    c = QTextCodec::codecForHtml(buf);
    QString buf_s(c->toUnicode(buf));

    //c = QTextCodec::codecForName("cp1251");

    QRegExp regexp("id_film[\\s]=[\\s]([^\\D]\\d+)+\\D");
    int pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:code_film_kp}"] = QString::fromLocal8Bit(regexp.cap(1).toLocal8Bit());
    }

    if (!FmapTags["{:code_film_kp}"].isEmpty()) {
        FmapTags["{:url_kp}"] = "http://www.kinopoisk.ru/film/"+FmapTags["{:code_film_kp}"]+"/";
    }

    regexp.setPattern("<h1 class=\"moviename-big\" itemprop=\"name\">+([^<]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:title_ru}"] = html_decode(regexp.cap(1));
        FmapTags["{:title_ru}"].replace(QRegExp("\\s$"),"");
    }

    regexp.setPattern("<span itemprop=\"alternativeHeadline\">+([^<]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:title_eng}"] = html_decode(regexp.cap(1));
    }

    regexp.setPattern("год</td>+[^h]+[^>]+>+([^<]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:year}"] = html_decode(regexp.cap(1));
    }

    regexp.setPattern("бюджет</td>+[^h]+[^>]+>+([^<]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:budget}"] = html_decode(regexp.cap(1));
    }

    regexp.setPattern("композитор</td>+[^>]+>+([^\\n]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp.cap(1);
        QRegExp _rr("<a+[^>]+>+([^<]*)</a>");

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:compositor}"].isEmpty()) FmapTags["{:compositor}"].append(",");
                FmapTags["{:compositor}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    regexp.setPattern("страна</td>+[^<]+<td><+[^<]+([^\\n]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp.cap(1);
        QRegExp _rr("<a+[^>]+>+([^<]*)</a>");

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {
            if(_rr.cap(1)!="...") {                
                if (!FmapTags["{:country}"].isEmpty()) FmapTags["{:country}"].append(",");
                FmapTags["{:country}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    regexp.setPattern("режиссер</td>+[^>]+>+([^\\n]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp.cap(1);
        QRegExp _rr("<a+[^>]+>+([^<]*)</a>");

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:director}"].isEmpty()) FmapTags["{:director}"].append(",");
                FmapTags["{:director}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    regexp.setPattern("GetTrailerPreview\\(\\{+[^,]+[\\D]+(\\d+)+[\\D]+(\\d+)+[^:]+[^\"]+\"+([^\"]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:trailer}"] = QString("<OBJECT><EMBED src=\"http://trailers.kinopoisk.ru/js"
                                "/jw/player-licensed.swf\" type=\"application/x-shockwave-flash\" "
                                "wmode=\"transparent\" width=\"%1\" height=\"%2\" "
                                "allowfullscreen=\"true\" flashvars=\"file=http://www.kinopoisk.ru/gettrailer.php?id=%3"
                                "\"></EMBED></OBJECT>").arg(regexp.cap(1)).arg(regexp.cap(2)).
                                arg(FmapTags["{:code_film_kp}"]+"%26tid="+regexp.cap(3));
    }

    regexp.setPattern("<span itemprop=\"genre\">+([^\\n]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp.cap(1);
        QRegExp _rr("<a+[^>]+>+([^<]*)</a>");

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:genre}"].isEmpty()) FmapTags["{:genre}"].append(",");
                FmapTags["{:genre}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    regexp.setPattern("IMDb:+\\s+([^<]*)+<");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:rating_imbd}"] = regexp.cap(1);
    }

    regexp.setPattern("<span class=\"rating_ball\">+([^<]*)+[^>]+>+[^>]+>+([^<]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:rating_kp}"] = regexp.cap(1)+" ("+regexp.cap(2)+")";
        FmapTags["{:rating_kp}"].replace(QRegExp("&nbsp;"),"");
    }

    regexp.setPattern("оператор</td>+[^>]+>+([^\\n]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp.cap(1);
        QRegExp _rr("<a+[^>]+>+([^<]*)</a>");

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {            
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:operator}"].isEmpty()) FmapTags["{:operator}"].append(",");
                FmapTags["{:operator}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    QRegExp regexp_desc("_reachbanner_+[^>]+>+[^>]+>(.*)(?=(</div>))");
    regexp_desc.setMinimal(true);
    pos = regexp_desc.indexIn(buf_s,0);
    if(pos!=(-1)) {        
        FmapTags["{:desc}"] = html_decode(regexp_desc.cap(1));
        QRegExp regexp_br("<br.+>");
        regexp_br.setMinimal(true);
        FmapTags["{:desc}"] = FmapTags["{:desc}"].replace(regexp_br,"");
        FmapTags["{:desc}"] = FmapTags["{:desc}"].replace("<","");
    }

    regexp.setPattern("data-ical-type=\"rus\"+[^\"]+\"+([^\"]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {        
        FmapTags["{:premier_ru}"] = html_decode(regexp.cap(1));
    }

    regexp.setPattern("data-ical-type=\"world\"+[^\"]+\"+([^\"]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:premier_world}"] = html_decode(regexp.cap(1));
    }

    regexp.setPattern("продюсер</td>+[^>]+>+([^\\n]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp.cap(1);
        QRegExp _rr("<a+[^>]+>+([^<]*)</a>");

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {            
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:producer}"].isEmpty()) FmapTags["{:producer}"].append(",");
                FmapTags["{:producer}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    QRegExp regexp_mraa("<td class=\"rate+(.*)(?=(alt=\"))+alt=\"+([^\"]*)+\"");
    regexp_mraa.setMinimal(true);
    pos = regexp_mraa.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:rating_MPAA}"] = html_decode(regexp_mraa.cap(2));
    }

    regexp.setPattern("data-ical-type=\"dvd\"+[^\"]+\"+([^\"]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:release_DVD}"] = html_decode(regexp.cap(1));
    }

    regexp.setPattern("сценарий</td>+[^>]+>+([^\\n]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp.cap(1);
        QRegExp _rr("<a+[^>]+>+([^<]*)</a>");

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:scenario}"].isEmpty()) FmapTags["{:scenario}"].append(",");
                FmapTags["{:scenario}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    regexp.setPattern("слоган</td>+[^>]+>+([^<]*)+</td>");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:slogan}"] = html_decode(regexp.cap(1));
    }

    regexp.setPattern("время</td>+[^>]+>+([^<]*)");
    pos = regexp.indexIn(buf_s,0);
    if(pos!=(-1)) {
        FmapTags["{:time}"] = html_decode(regexp.cap(1));
    }

    QRegExp regexp_actors1("В главных ролях:+.+<ul>+(.*)+</ul>");
    regexp_actors1.setMinimal(true);
    pos = regexp_actors1.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp_actors1.cap(1);
        QRegExp _rr("<a+[^>]+>+(.*)+</a>");
        _rr.setMinimal(true);

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:actor}"].isEmpty()) FmapTags["{:actor}"].append(",");
                FmapTags["{:actor}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    QRegExp regexp_dubl("Роли дублировали:+.+<ul>+(.*)+</ul>");
    regexp_dubl.setMinimal(true);
    pos = regexp_dubl.indexIn(buf_s,0);
    if(pos!=(-1)) {
        QString _ss = regexp_dubl.cap(1);
        QRegExp _rr("<a+[^>]+>+(.*)+</a>");
        _rr.setMinimal(true);

        int i = _rr.indexIn(_ss,0);
        while(i!=(-1)) {
            if(_rr.cap(1)!="...") {
                if (!FmapTags["{:dublicate}"].isEmpty()) FmapTags["{:dublicate}"].append(",");
                FmapTags["{:dublicate}"].append(html_decode(_rr.cap(1)));
            }
            i = _rr.indexIn(_ss,i+1);
        }
    }

    if (FmapTags["{:title_eng}"].isEmpty()) FmapTags["{:title_eng_ru}"] = FmapTags["{:title_ru}"];
    else FmapTags["{:title_eng_ru}"] = FmapTags["{:title_eng}"];

    FmapTags["{:title_eng_clear}"] = FmapTags["{:title_eng}"];
    FmapTags["{:title_eng_clear}"].replace(QRegExp("\\W"),"");
    FmapTags["{:title_eng_clear}"] = FmapTags["{:title_eng_clear}"].toLower();

    emit m_Notifyer->signalPars(0,"");
}

QString PluginSearchKP::html_decode(QString stroka) {
    QString pars_string = stroka;
    QRegExp r_ht("&([^;]*);");

    int pos = r_ht.indexIn(pars_string,0);
    while(pos!=(-1)) {
        QString html_c = r_ht.cap(0);
        QRegExp r_g("#([\\d]*);");
        if(r_g.indexIn(html_c,0)!=(-1)) {
            QString ss(" ");
            //QTextCodec *c;
            //c = QTextCodec::codecForName("cp1251");
            switch(r_g.cap(1).toInt()) {
                case 151: {
                    ss = "—";
                    break;
                }
                case 133: {
                    ss = "…";
                    break;
                }
                case 150: {
                    ss = "-";
                    break;
                }
                case 123: {
                    ss = "{";
                    break;
                }
                case 125: {
                    ss = "}";
                    break;
                }
                default: {
                    ss = QChar(r_g.cap(1).toInt());
                    break;
                }
            }
            pars_string.replace(html_c,ss);
        }
        else {
            QChar ss(' ');
            if(html_c=="&quot;") ss = QChar(34);
            if(html_c=="&amp;") ss = QChar(38);
            if(html_c=="&lt;") ss = QChar(60);
            if(html_c=="&gt;") ss = QChar(62);
            if(html_c=="&nbsp;") ss = QChar(160);
            if(html_c=="&iexcl;") ss = QChar(161);
            if(html_c=="&cent;") ss = QChar(162);
            if(html_c=="&pound;") ss = QChar(163);
            if(html_c=="&curren;") ss = QChar(164);
            if(html_c=="&yen;") ss = QChar(165);
            if(html_c=="&brvbar;") ss = QChar(166);
            if(html_c=="&sect;") ss = QChar(167);
            if(html_c=="&uml;") ss = QChar(168);
            if(html_c=="&copy;") ss = QChar(169);
            if(html_c=="&ordf;") ss = QChar(170);
            if(html_c=="&laquo;") ss = QChar(171);
            if(html_c=="&not;") ss = QChar(172);
            if(html_c=="&shy;") ss = QChar(173);
            if(html_c=="&reg;") ss = QChar(174);
            if(html_c=="&macr;") ss = QChar(175);
            if(html_c=="&deg;") ss = QChar(176);
            if(html_c=="&plusmn;") ss = QChar(177);
            if(html_c=="&sup2;") ss = QChar(178);
            if(html_c=="&sup3;") ss = QChar(179);
            if(html_c=="&acute;") ss = QChar(180);
            if(html_c=="&micro;") ss = QChar(181);
            if(html_c=="&para;") ss = QChar(182);
            if(html_c=="&middot;") ss = QChar(183);
            if(html_c=="&cedil;") ss = QChar(184);
            if(html_c=="&sup1;") ss = QChar(185);
            if(html_c=="&ordm;") ss = QChar(186);
            if(html_c=="&raquo;") ss = QChar(187);
            if(html_c=="&frac14;") ss = QChar(188);
            if(html_c=="&frac12;") ss = QChar(189);
            if(html_c=="&frac34;") ss = QChar(190);
            if(html_c=="&iquest;") ss = QChar(191);
            if(html_c=="&Agrave;") ss = QChar(192);
            if(html_c=="&Aacute;") ss = QChar(193);
            if(html_c=="&Acirc;") ss = QChar(194);
            if(html_c=="&Atilde;") ss = QChar(195);
            if(html_c=="&Auml;") ss = QChar(196);
            if(html_c=="&Aring;") ss = QChar(197);
            if(html_c=="&AElig;") ss = QChar(198);
            if(html_c=="&Ccedil;") ss = QChar(199);
            if(html_c=="&Egrave;") ss = QChar(200);
            if(html_c=="&Eacute;") ss = QChar(201);
            if(html_c=="&Ecirc;") ss = QChar(202);
            if(html_c=="&Euml;") ss = QChar(203);
            if(html_c=="&Igrave;") ss = QChar(204);
            if(html_c=="&Iacute;") ss = QChar(205);
            if(html_c=="&Icirc;") ss = QChar(206);
            if(html_c=="&Iuml;") ss = QChar(207);
            if(html_c=="&ETH;") ss = QChar(208);
            if(html_c=="&Ntilde;") ss = QChar(209);
            if(html_c=="&Ograve;") ss = QChar(210);
            if(html_c=="&Oacute;") ss = QChar(211);
            if(html_c=="&Ocirc;") ss = QChar(212);
            if(html_c=="&Otilde;") ss = QChar(213);
            if(html_c=="&Ouml;") ss = QChar(214);
            if(html_c=="&times;") ss = QChar(215);
            if(html_c=="&Oslash;") ss = QChar(216);
            if(html_c=="&Ugrave;") ss = QChar(217);
            if(html_c=="&Uacute;") ss = QChar(218);
            if(html_c=="&Ucirc;") ss = QChar(219);
            if(html_c=="&Uuml;") ss = QChar(220);
            if(html_c=="&Yacute;") ss = QChar(221);
            if(html_c=="&THORN;") ss = QChar(222);
            if(html_c=="&szlig;") ss = QChar(223);
            if(html_c=="&agrave;") ss = QChar(224);
            if(html_c=="&aacute;") ss = QChar(225);
            if(html_c=="&acirc;") ss = QChar(226);
            if(html_c=="&atilde;") ss = QChar(227);
            if(html_c=="&auml;") ss = QChar(228);
            if(html_c=="&aring;") ss = QChar(229);
            if(html_c=="&aelig;") ss = QChar(230);
            if(html_c=="&ccedil;") ss = QChar(231);
            if(html_c=="&egrave;") ss = QChar(232);
            if(html_c=="&eacute;") ss = QChar(233);
            if(html_c=="&ecirc;") ss = QChar(234);
            if(html_c=="&euml;") ss = QChar(235);
            if(html_c=="&igrave;") ss = QChar(236);
            if(html_c=="&iacute;") ss = QChar(237);
            if(html_c=="&icirc;") ss = QChar(238);
            if(html_c=="&iuml;") ss = QChar(239);
            if(html_c=="&eth;") ss = QChar(240);
            if(html_c=="&ntilde;") ss = QChar(241);
            if(html_c=="&ograve;") ss = QChar(242);
            if(html_c=="&oacute;") ss = QChar(243);
            if(html_c=="&ocirc;") ss = QChar(244);
            if(html_c=="&otilde;") ss = QChar(245);
            if(html_c=="&ouml;") ss = QChar(246);
            if(html_c=="&divide;") ss = QChar(247);
            if(html_c=="&oslash;") ss = QChar(248);
            if(html_c=="&ugrave;") ss = QChar(249);
            if(html_c=="&uacute;") ss = QChar(250);
            if(html_c=="&ucirc;") ss = QChar(251);
            if(html_c=="&uuml;") ss = QChar(252);
            if(html_c=="&yacute;") ss = QChar(253);
            if(html_c=="&thorn;") ss = QChar(254);
            if(html_c=="&yuml;") ss = QChar(255);
            if(html_c=="&euro;") ss = QChar(8364);

            pars_string.replace(html_c,QString(ss));
        }
        pos = r_ht.indexIn(pars_string,0);
    }

    return pars_string;
}

QList<QString> PluginSearchKP::listTags() {
    QList<QString> ret;

    QMapIterator<QString,QString> i(FmapTags);
    i.toFront();
    while (i.hasNext()) {
        i.next();
        ret.append(i.key());
    }
    return ret;
}

QString PluginSearchKP::result_tags(QString tt) {
    QMap<QString,QString>::const_iterator i = FmapTags.find(tt);
    if (i.key() == tt) return i.value();
    else return "";
}

void PluginSearchKP::pars_small_image(QString code_film, QByteArray buf) {
    QTextCodec *c;
    c = QTextCodec::codecForHtml(buf);
    QString buf_s(c->toUnicode(buf));
    QList<QString> res_sm;

    //картинка main:
    //small - http://st.kp.yandex.net/images/sm_film/{code_film}.jpg
    //full - http://st.kp.yandex.net/images/film_big/{code_film}.jpg

    sm_image tmp_sm;
    tmp_sm.id = QString("main_%1").arg(code_film);
    tmp_sm.url = QString("http://st.kp.yandex.net/images/sm_film/%1.jpg").arg(code_film);
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
        download_image("main",Fdir_temp);
    }
    res_sm.append(QString("%1").arg(tmp_sm.id));

    QRegExp regexp("<a href=\"/picture/+([\\d]*)+/\"><img+.+\"+([^\"]*)+\"");
    regexp.setMinimal(true);
    int pos = regexp.indexIn(buf_s,0);
    while (pos!=(-1)) {
        sm_image tmp_sm;
        tmp_sm.id = regexp.cap(1);
        tmp_sm.url = regexp.cap(2);
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
            //Fnum_com = 3;
            http_download *hd_search = new http_download(this,tmp_sm.url,3,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();
        }

        pos = regexp.indexIn(buf_s,pos+1);

        res_sm.append(QString("%1").arg(tmp_sm.id));
    }

    emit m_Notifyer->signalSmallImage(0,res_sm);

    res_sm.clear();
}

void PluginSearchKP::pars_image(QByteArray buf) {
    QTextCodec *c;
    c = QTextCodec::codecForHtml(buf);
    QString buf_s(c->toUnicode(buf));

    QRegExp regexp("id=\"image\" src=\"+([^\"]*)+\"");
    regexp.setMinimal(true);
    int pos = regexp.indexIn(buf_s,0);

    if (pos != (-1)) {
        http_download *hd_search = new http_download(this,regexp.cap(1),5,Fproxy,Fhost,Fport,Fusername,Fpassword);
        connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
        hd_search->_download();
    }
}

void PluginSearchKP::download_image(QString id,QString dir_tmp) {
    Fdir_temp = dir_tmp;
    if (!QFile(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_temp).arg(id))).exists()) {
        http_download *hd_search;
        if (id.indexOf("main")!=(-1)) {
            QRegExp rr("\\d+");
            if (rr.indexIn(id)!=(-1)) {
                hd_search = new http_download(this,QString("http://st.kp.yandex.net/images/film_big/%1.jpg").arg(rr.cap(0)),6,Fproxy,Fhost,Fport,Fusername,Fpassword);
            } else return;
        } else hd_search = new http_download(this,QString("http://www.kinopoisk.ru/picture/%1/").arg(id),4,Fproxy,Fhost,Fport,Fusername,Fpassword);
        connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
        hd_search->_download();
    } else {
        emit m_Notifyer->signalDownloadImage(id,0);
    }
}

void PluginSearchKP::download_all_image(QString dir_tmp) {
    Fdir_temp = dir_tmp;
    for (int i = 0; i < FlistSmallImageFull.count(); i++) {
        if (!QFile(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_temp).arg(FlistSmallImageFull[i].id))).exists()) {
            http_download *hd_search;
            if (FlistSmallImageFull[i].id.indexOf("main")!=(-1)) {
                QRegExp rr("\\d+");
                if (rr.indexIn(FlistSmallImageFull[i].id)!=(-1)) {
                    hd_search = new http_download(this,QString("http://st.kp.yandex.net/images/film_big/%1.jpg").arg(rr.cap(0)),6,Fproxy,Fhost,Fport,Fusername,Fpassword);
                } else return;
            } else hd_search = new http_download(this,QString("http://www.kinopoisk.ru/picture/%1/").arg(FlistSmallImageFull[i].id),4,Fproxy,Fhost,Fport,Fusername,Fpassword);
            connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            hd_search->_download();
        } else {
            emit m_Notifyer->signalDownloadImage(FlistSmallImageFull[i].id,0);
        }
    }
}
