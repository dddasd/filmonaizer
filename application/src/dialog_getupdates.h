#ifndef DIALOG_GETUPDATES_H
#define DIALOG_GETUPDATES_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QMovie>
//#include "global_struct.h"

class dialog_getupdates : public QDialog
{
	Q_OBJECT
public:
	dialog_getupdates(QWidget *parent = 0);
	~dialog_getupdates();

signals:
	void cancel_download();
};

#endif // DIALOG_GETUPDATES_H
