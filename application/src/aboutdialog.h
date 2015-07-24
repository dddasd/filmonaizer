#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QTextCodec>
#include "ui_aboutdialog.h"
//#include "global_struct.h"
#include "dialog_getupdates.h"
#include "http_download.h"
#include "dialog_version.h"

class AboutDialog : public QDialog, private Ui::Form_about
{
    Q_OBJECT

public:
    AboutDialog(QString cur_l,bool prox = false,QString host = "",QString port = "",QString username = "",QString password = "",QWidget *parent = 0);

private:
	http_download *http_d;
	dialog_getupdates *dial;

    bool Fprox;
    QString Fhost;
    QString Fport;
    QString Fusername;
    QString Fpassword;

signals:
    void ch_locale(QString i);
	void save_version(QString vr, int vr_c);

private slots:
    void on_comboBox_activated(int index);
	void get_updates();
	void cancel_download();
	void fin_d_version(result_url ret_code);
};

#endif // ABOUTDIALOG_H
