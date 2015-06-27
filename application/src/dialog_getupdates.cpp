#include "dialog_getupdates.h"

dialog_getupdates::dialog_getupdates( QWidget *parent /*= 0*/ )
{
	Q_UNUSED(parent);
	this->setWindowTitle(tr("Search updates"));
	this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

	QVBoxLayout *boxx = new QVBoxLayout(this);
	QHBoxLayout *boxx1 = new QHBoxLayout(this);
	
	QLabel *l1 = new QLabel(this);
	QMovie *movie = new QMovie(":icons/loading.gif");
	l1->setMovie(movie);
	movie->start();

	boxx1->addWidget(l1);

	QLabel *l2 = new QLabel(tr("goes searching for of the updates programm"),this);
	boxx1->addWidget(l2);

	boxx->addLayout(boxx1);

	QHBoxLayout *boxx2 = new QHBoxLayout(this);
	QPushButton *but = new QPushButton(this);
	but->setText(tr("Cancel"));
	connect(but,SIGNAL(clicked()),this,SLOT(close()));
	boxx2->addStretch(10);
	boxx2->addWidget(but);
	boxx->addLayout(boxx2);
	this->setLayout(boxx);
}

dialog_getupdates::~dialog_getupdates()
{
	emit cancel_download();
}
