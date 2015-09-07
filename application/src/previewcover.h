#ifndef PREVIEWCOVER_H
#define PREVIEWCOVER_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QWheelEvent>
#include <QImageReader>
#include <QMessageBox>
#include "ui_preview_cover.h"
#include "http_download.h"
/*
class PreviewCover : public QDialog, public Ui::Form_preview_cover
{
    Q_OBJECT

public:
        PreviewCover(bool pr,proxy_l pr_,QList<QString> name_files,QList<int> check,QString dir_tmp,int count_clicked=0,QWidget *parent = 0);
        ~PreviewCover();
protected:
        void wheelEvent(QWheelEvent *event);
private:
        bool proxy;
        proxy_l proxy_list;

        QList<QString> list_file;
        QList<int> list_check;
        QString dir;
        qreal scale;
        bool zagr_im,im_download;
        int tek_img;

        QString zagr_img;

        void view_image(QString filename = "");

private slots:
    void on_pushButton_zoom_in_clicked();
    void on_pushButton_zoom_out_clicked();
    void on_pushButton_savefile_clicked();
    void fin_d(result_url ret_code);
    void _itemDoubleClicked(QListWidgetItem* ret);

signals:
    void check_stat(QList<int> check);
};
*/
#endif // PREVIEWCOVER_H
