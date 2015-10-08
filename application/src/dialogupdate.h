#ifndef DIALOGUPDATE_H
#define DIALOGUPDATE_H

#include <QMap>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "http_download.h"

#include "ui_dialog_updates.h"

#define url_update "https://raw.githubusercontent.com/dddasd/filmonaizer/master/version"

class DialogUpdate : public QDialog, public Ui::DialogUpdate
{
    Q_OBJECT
public:
    DialogUpdate(QMap<QString,QString> map_up, bool prox = false, QString host = "", QString port = "", QString username = "", QString password = "", QWidget *parent = 0);
    ~DialogUpdate();

private:
    QMap<QString,QString> Fmap_up;

    bool Fprox;
    QString Fhost;
    QString Fport;
    QString Fusername;
    QString Fpassword;

private slots:
    void fin_d(result_url);
};

#endif // DIALOGUPDATE_H
