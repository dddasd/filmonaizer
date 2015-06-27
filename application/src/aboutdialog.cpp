#include "aboutdialog.h"

AboutDialog::AboutDialog(int cur_l,bool prox,QString host,QString port,QString username,QString password,QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    setModal(true);

    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

    label_main->setText(label_main->text().arg(qApp->applicationVersion()).arg(QT_VERSION_STR));

    if (cur_l==0)
        comboBox->setCurrentIndex(1);
    else
        comboBox->setCurrentIndex(0);

	connect(pushButton_updates,SIGNAL(clicked()),this,SLOT(get_updates()));

    Fprox = prox;
    Fhost = host;
    Fport = port;
    Fusername = username;
    Fpassword = password;
}

void AboutDialog::on_comboBox_activated(int index)
{
    if (index!=(-1)) {
        switch (index) {
            case 0: {
                emit ch_locale(1);
                break;
            }
            case 1: {
                emit ch_locale(0);
                break;
            }
        }
        retranslateUi(this);
		label_main->setText(label_main->text().arg(qApp->applicationVersion()).arg(QT_VERSION_STR));
        comboBox->setCurrentIndex(index);
    }
}

void AboutDialog::get_updates()
{
    http_d = new http_download(this,"http://wiki.soulruins.info/filmonaizer/version",Fprox,Fhost,Fport,Fusername,Fpassword);
	connect(http_d,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d_version(result_url)));
	http_d->_download();

	dial = new dialog_getupdates(this);
	dial->setAttribute(Qt::WA_DeleteOnClose);
	connect(dial,SIGNAL(cancel_download()),this,SLOT(cancel_download()));
	dial->exec();
}

void AboutDialog::cancel_download()
{
	disconnect(http_d,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d_version(result_url)));
	http_d->deleteLater();
	disconnect(dial,SIGNAL(cancel_download()),this,SLOT(cancel_download()));
	dial->close();
}

void AboutDialog::fin_d_version( result_url ret_code )
{
	disconnect(dial,SIGNAL(cancel_download()),this,SLOT(cancel_download()));
	dial->close();

	if (ret_code.code_r==0) {
		QTextCodec *c;
		c = QTextCodec::codecForHtml(ret_code.buf_d);
		QString buf_r(c->toUnicode(ret_code.buf_d));

		QRegExp reg_exp("\\{;version=([^\\}]*)\\}");
		if (reg_exp.indexIn(buf_r,0)!=(-1))	{
			if (reg_exp.cap(1)!=qApp->applicationVersion()) {
				QRegExp reg_exp2("([\\d]*).([\\d]*).([\\d]*)");
				if (reg_exp2.indexIn(reg_exp.cap(1),0)!=(-1)) {
					int v1,v2,v3;
					v1 = reg_exp2.cap(1).toInt();
					v2 = reg_exp2.cap(2).toInt();
					v3 = reg_exp2.cap(3).toInt();

					if (reg_exp2.indexIn(qApp->applicationVersion(),0)!=(-1)) {
						int v11,v21,v31;
						v11 = reg_exp2.cap(1).toInt();
						v21 = reg_exp2.cap(2).toInt();
						v31 = reg_exp2.cap(3).toInt();
						if ((v1>=v11) && (v2>=v21) && (v3>=v31))
						{
							reg_exp2.setPattern("\\{;description_version=([^\\}]*)\\}");
							QString d_vr = "";
							if (reg_exp2.indexIn(buf_r,0)!=(-1)) {
								d_vr = reg_exp2.cap(1);
							}

							dialog_version *dial1 = new dialog_version(reg_exp.cap(1),d_vr,this);
							dial1->setAttribute(Qt::WA_DeleteOnClose);
							connect(dial1,SIGNAL(save_version(QString,int)),this,SLOT(save_version(QString,int)));
							dial1->show();
						}
						else {
							QMessageBox msgBox;
							msgBox.setWindowTitle(tr("Updates"));
							msgBox.setText(tr("No updates. You use the last version programm."));
							msgBox.setIcon(QMessageBox::Information);
							msgBox.setStandardButtons(QMessageBox::Ok);
							msgBox.setDefaultButton(QMessageBox::Ok);
							msgBox.exec();
						}
					}
					else {
						QMessageBox msgBox;
						msgBox.setWindowTitle(tr("Updates"));
						msgBox.setText(tr("Not to manage to get the version of the programm"));
						msgBox.setIcon(QMessageBox::Warning);
						msgBox.setStandardButtons(QMessageBox::Ok);
						msgBox.setDefaultButton(QMessageBox::Ok);
						msgBox.exec();
					}
				}
				else {
					QMessageBox msgBox;
					msgBox.setWindowTitle(tr("Updates"));
					msgBox.setText(tr("Not to manage to get the version of the programm"));
					msgBox.setIcon(QMessageBox::Warning);
					msgBox.setStandardButtons(QMessageBox::Ok);
					msgBox.setDefaultButton(QMessageBox::Ok);
					msgBox.exec();
				}
			}
			else{
				QMessageBox msgBox;
				msgBox.setWindowTitle(tr("Updates"));
				msgBox.setText(tr("Not to manage to get the version of the programm"));
				msgBox.setIcon(QMessageBox::Warning);
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.setDefaultButton(QMessageBox::Ok);
				msgBox.exec();
			}
		}
		else {
			QMessageBox msgBox;
			msgBox.setWindowTitle(tr("Updates"));
			msgBox.setText(tr("Not to manage to get the version of the programm"));
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.exec();
		}
	}
	else {
		QMessageBox msgBox;
		msgBox.setWindowTitle(tr("Updates"));
		msgBox.setText(tr("Not to manage to get the version of the programm"));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
	}
}
