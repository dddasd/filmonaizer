#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QTextCodec>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog, private Ui::Form_about
{
    Q_OBJECT
public:
    AboutDialog(QString cur_l, QWidget *parent = 0);

signals:
    void ch_locale(QString i);

private slots:
    void on_comboBox_activated(int index);
};

#endif // ABOUTDIALOG_H
