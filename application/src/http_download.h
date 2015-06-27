#ifndef HTTP_DOWNLOAD_H
#define HTTP_DOWNLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QFile>
#include <QDir>
#include <QDebug>

struct result_url {
    QString url;
    QString url_redirect;
    QString error_string;
    int code_r;
    QByteArray buf_d;
};

class http_download: public QObject {
    Q_OBJECT
public:
    http_download(QObject *parent, QString url, bool prox = false, QString host = "", QString port = "", QString username = "", QString password = "");
    void _download();

private:
    result_url _res;

    QNetworkAccessManager *manager;
    QNetworkProxy proxy;

signals:
    void fin_potok(result_url);

private slots:
    void replyFinished(QNetworkReply*);
};

#endif // HTTP_DOWNLOAD_H
