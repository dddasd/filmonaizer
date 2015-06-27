#ifndef PREVIEWDESC_H
#define PREVIEWDESC_H

#include <QDialog>
#include <QFileDialog>
#include <QClipboard>
#include <QMessageBox>
#include <QDesktopServices>
#include "ui_preview_desc.h"

class Previewdesc : public QDialog, public Ui::Form_preview_desc
{
    Q_OBJECT
public:
    Previewdesc(QString desc, QString link, QList<QString> templ, int current_templ, QWidget *parent = 0);

private:
	QString link_kp;

signals:
	QString template_change(QString templ);

private slots:
    void on_pushButton_save_file_clicked();
	void on_pushButton_save_buffer_clicked();
	void on_pushButton_save_twitter_clicked();
	void on_pushButton_save_buzz_clicked();
	void on_pushButton_save_friendfeed_clicked();
	void on_pushButton_save_vkontakte_clicked();
	void on_pushButton_save_facebook_clicked();
    void on_pushButton_close_clicked();
	void desc_change(QString templ);
};

#endif // PREVIEWDESC_H
