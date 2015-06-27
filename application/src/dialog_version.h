#ifndef DIALOG_VERSION_H
#define DIALOG_VERSION_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QCheckBox>
#include <QString>
#include <QApplication>
//#include "global_struct.h"

class dialog_version : public QDialog
{
	Q_OBJECT
public:
	dialog_version(QString vr, QString d_vr, QWidget *parent = 0);
	~dialog_version();

private:
	QString ver;
	QCheckBox *checkbox;

signals:
	void save_version(QString vr, int vr_c);
};

#endif // DIALOG_VERSION_H
