#ifndef PREVIEWDESC_H
#define PREVIEWDESC_H

#include <QDialog>
#include <QFileDialog>
#include <QClipboard>
#include <QMessageBox>
#include <QMenu>
#include <QDesktopServices>
#include "ui_preview_desc.h"

class Previewdesc : public QDialog, public Ui::Form_preview_desc
{
    Q_OBJECT
public:
    Previewdesc(QString desc, QString link, QList<QString> templ, int current_templ, QString current_local, QWidget *parent = 0);

private:
	QString link_kp;

    QMenu *menu_export;

    void create_menu_export(QString cur_local);

signals:
	QString template_change(QString templ);

private slots:
    void on_pushButton_save_file_clicked();
    void on_pushButton_save_buffer_clicked();
    void on_pushButton_close_clicked();
	void desc_change(QString templ);
    void on_menu_export_triggered(QAction*);
};

#endif // PREVIEWDESC_H
