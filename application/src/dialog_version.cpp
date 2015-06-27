#include "dialog_version.h"


dialog_version::dialog_version( QString vr, QString d_vr, QWidget *parent /*= 0*/ )
: QDialog(parent)
{
	ver = vr;

	this->setWindowTitle(tr("Update"));
	this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

	QVBoxLayout *boxx = new QVBoxLayout(this);
	QLabel *l1 = new QLabel(QString(tr("A new version of the program to download, the current version - %1, available version - %2.")).arg(qApp->applicationVersion()).arg(ver),this);
	boxx->addWidget(l1);
	QLabel *l2 = new QLabel(QString(tr("<a href=\"http://wiki.soulruins.info/filmonaizer/downloads\">Go to download page...</a>")),this);
	l2->setOpenExternalLinks(true);
	boxx->addWidget(l2);

	QString ss = QString(tr("What is new:\n\n%1")).arg(d_vr);

	QTextBrowser *l3 = new QTextBrowser(this);
	l3->setText(ss);
	boxx->addWidget(l3);

	checkbox = new QCheckBox(this);
	checkbox->setChecked(true);
	checkbox->setText(tr("do not remind about the update"));
	boxx->addWidget(checkbox);

	QPushButton *but = new QPushButton(this);
	but->setText(tr("Close"));
	connect(but,SIGNAL(clicked()),this,SLOT(close()));
	boxx->addWidget(but);
	this->setLayout(boxx);
}

dialog_version::~dialog_version()
{
	if(checkbox->checkState()==Qt::Checked) {
		save_version(ver,1);
	}
	else {
		save_version(ver,0);
	}
}