#include "http_download.h"

http_download::http_download(QObject *parent, QString url, bool prox, QString host, QString port, QString username, QString password) {
	Q_UNUSED(parent);
    _res.url = url;
    _res.url_redirect.clear();
    _res.error_string.clear();
    _res.buf_d.clear();

    qDebug() << url;

    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

    if (prox) {
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(host);
        proxy.setPort(port.toInt());
        proxy.setUser(username);
        proxy.setPassword(password);
        manager->setProxy(proxy);
    }
}

void http_download::_download() {
    _res.buf_d.clear();
    QUrl _u(_res.url);
    if(_u.isValid()) {
        //формируем запрос
        QNetworkRequest request;
        request.setUrl(_u);

        request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows; U; MSIE 7.0; Windows NT 6.0; en-US)");
        request.setRawHeader("Accept", "text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");
        request.setRawHeader("Timeout", "5000");

        //Отправляем запрос
        manager->get(request);
    }
}

void http_download::replyFinished(QNetworkReply* reply) {
    if(reply->error()) {
        _res.error_string = tr("error code http: ") + reply->errorString();
        qDebug() << _res.error_string;

        _res.code_r=1;
    }
    else {
        QVariant code_reply = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << code_reply;
        switch(code_reply.toInt()) {
            case 301: {
                _res.error_string = tr("error code http: ") + QString("%1").arg(code_reply.toInt()) + tr(" (Moved Permanently)");
                qDebug() <<  _res.error_string;
                _res.code_r=301;
                break;
            }
            case 302: {
                _res.error_string = tr("error code http: ") + QString("%1").arg(code_reply.toInt()) + tr(" (Found) ") +
                                    tr("url_header: ") + reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
                qDebug() << _res.error_string;
                _res.code_r=302;
                _res.url_redirect=reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
                break;
            }
            case 303: {
                _res.error_string = tr("error code http: ") + QString("%1").arg(code_reply.toInt()) + tr(" (See Other)");
                qDebug() << _res.error_string;
                _res.code_r=303;
                break;
            }
            case 307: {
                _res.error_string = tr("error code http: ") + QString("%1").arg(code_reply.toInt()) + tr(" (Temporary Redirect)");
                qDebug() << _res.error_string;
                _res.code_r=307;
                break;
            }
            case 200: {
                qDebug() << "reply ok";
                _res.code_r=0;
				_res.buf_d.append(reply->readAll());
                break;
            }
            default: {
                _res.error_string = tr("error code http: ") + QString("%1").arg(code_reply.toInt());
                _res.code_r=2;
            }
        }
    }

    reply->deleteLater();
    fin_potok(_res);
    this->deleteLater();
}
