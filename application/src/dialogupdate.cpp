#include "dialogupdate.h"

struct updat {
    QString name;
    QString version;
    QString changes;
    bool bool_v;
};

DialogUpdate::DialogUpdate(QMap<QString,QString> map_up, bool prox, QString host, QString port, QString username, QString password, QWidget *parent) : QDialog(parent) {
    setupUi(this);

    Fmap_up = map_up;
    Fprox = prox;
    Fhost = host;
    Fport = port;
    Fusername = username;
    Fpassword = password;

    textBrowserUpdate->append(tr("Search updates......"));

    http_download *hd_search = new http_download(this,url_update,0,Fprox,Fhost,Fport,Fusername,Fpassword);
    connect(hd_search,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
    hd_search->_download();
}

DialogUpdate::~DialogUpdate() {

}

void DialogUpdate::fin_d(result_url res) {
    textBrowserUpdate->clear();
    if (res.code_r != 0) {
        textBrowserUpdate->append(tr("When searching the update error occurred."));
        textBrowserUpdate->append(QString(tr("Error: %1")).arg(res.error_string));
        return;
    } else {
        QJsonParseError ir;
        QJsonDocument jDoc = QJsonDocument::fromJson(res.buf_d,&ir);

        if (ir.error != 0) {
            textBrowserUpdate->append(tr("When processing an update error occurred."));
            textBrowserUpdate->append(QString(tr("Error: %1")).arg(ir.errorString()));
            return;
        }

        QJsonObject jOb = jDoc.object();

        QList<updat> Sear_Upd;
        updat temp_upd;
        //main
        QJsonObject jObMain = jOb["main"].toObject();
        temp_upd.name = "main";
        temp_upd.version = jObMain["version"].toString();
        temp_upd.changes = jObMain["changes"].toString();
        temp_upd.bool_v = false;

        Sear_Upd << temp_upd;

        //plugins
        QJsonArray jAr = jOb["plugins"].toArray();
        for (int i = 0; i < jAr.count(); i++) {
            QJsonObject jObTemp = jAr[i].toObject();
            temp_upd.name = jObTemp["name"].toString();
            temp_upd.changes = jObTemp["changes"].toString();
            temp_upd.version = jObTemp["version"].toString();
            temp_upd.bool_v = false;

            Sear_Upd << temp_upd;
        }

        //compare
        QMapIterator<QString,QString> j(Fmap_up);
        j.toFront();
        while (j.hasNext()) {
            j.next();

            for (int i = 0; i < Sear_Upd.count(); i++) {
                if (j.key() == Sear_Upd[i].name) {
                    if (j.value() == Sear_Upd[i].version) {
                        Sear_Upd[i].bool_v = true;
                    }
                    break;
                }
            }
        }

        //
        bool b = true;
        for (int i = 0; i < Sear_Upd.count(); i++) {
            if (!Sear_Upd[i].bool_v) {
                b = false;
                break;
            }
        }

        if (!b) {
            textBrowserUpdate->append(tr("We found the following updates:"));
            for (int i = 0; i < Sear_Upd.count(); i++) {
                if (!Sear_Upd[i].bool_v) {
                    if (Sear_Upd[i].name == "main") {
                        textBrowserUpdate->append(tr("Main programm"));
                        textBrowserUpdate->append(QString(tr("Version: %1")).arg(Sear_Upd[i].version));
                        textBrowserUpdate->append(QString(tr("Changes: %1")).arg(Sear_Upd[i].changes));
                        textBrowserUpdate->append("");
                    } else {
                        textBrowserUpdate->append(QString(tr("Plugin name: %1")).arg(Sear_Upd[i].name));
                        textBrowserUpdate->append(QString(tr("Version: %1")).arg(Sear_Upd[i].version));
                        textBrowserUpdate->append(QString(tr("Changes: %1")).arg(Sear_Upd[i].changes));
                        textBrowserUpdate->append("");
                    }
                }
            }
            textBrowserUpdate->append(QString(tr("Link update: %1")).arg(jOb["link_updates"].toString()));
        } else {
            textBrowserUpdate->append(tr("Updates not found."));
        }
    }
}
