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
#include <QDebug>
#include "ui_preview_cover.h"

class PreviewCover : public QDialog, public Ui::Form_preview_cover
{
    Q_OBJECT
public:
        PreviewCover(QList<int> id_files, QList<int> check, QString dir_tmp, int id_clicked=0, QWidget *parent = 0);
        ~PreviewCover();

        void DownloadComplete(int);
protected:
        void wheelEvent(QWheelEvent *event);
private:
        QList<int> Fid_files;
        QList<int> Fcheck;
        QString Fdir_tmp;
        qreal scale;
        int FcurrentImage;
        void view_image(int id_filename = 0);
private slots:
    void on_pushButton_zoom_in_clicked();
    void on_pushButton_zoom_out_clicked();
    void on_pushButton_savefile_clicked();
    void itemClicked(QListWidgetItem* ret);

signals:
    void check_stat(QList<int> check);
    void download_image(int id);
};

#endif // PREVIEWCOVER_H
