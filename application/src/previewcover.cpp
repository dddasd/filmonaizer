#include "previewcover.h"

PreviewCover::PreviewCover(QList<QString> id_files, QList<int> check, QString dir_tmp, QString id_clicked, QWidget *parent) : QDialog(parent) {
    setupUi(this);
    setModal(true);

    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

    QList<QByteArray> rr = QImageReader::supportedImageFormats();
    qDebug() << rr;
    int ii = 0;
    for(int i = 0;i < rr.length();i++)
        if ((rr[i] == "jpg") | (rr[i] == "jpeg")) {
            ii = 1;
            break;
        }
    if (ii == 0) {
        QMessageBox msgBox;
        msgBox.setText(tr("Impossible viewing jpeg-files."));
        msgBox.setDetailedText(tr("No necessary libraries."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        close();
    }

    cover_View->setRenderHint(QPainter::Antialiasing, false);
    cover_View->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    cover_View->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    Fid_files = id_files;
    Fcheck = check;
    FcurrentImage = id_clicked;

    list_image_sm->clear();
    list_image_sm->setViewMode(QListView::IconMode);
    list_image_sm->setFlow(QListView::TopToBottom);
    list_image_sm->setIconSize(QSize(128, 128));
    list_image_sm->scrollBarWidgets(Qt::AlignHorizontal_Mask);

    for(int i = 0;i < Fid_files.length();i++) {
        QListWidgetItem *newAC = new QListWidgetItem(list_image_sm);
        newAC->setFlags(newAC->flags() | Qt::ItemIsUserCheckable);
        if (Fcheck[i]==0)
            newAC->setCheckState(Qt::Checked);
        else
            newAC->setCheckState(Qt::Unchecked);
        newAC->setText(QString("%1").arg(Fid_files[i]));
        //newAC->setSizeHint(QSize((list_image_sm->width()),((list_image_sm->height()))));
        list_image_sm->addItem(newAC);
        if (FcurrentImage == Fid_files[i]) list_image_sm->setCurrentItem(newAC,QItemSelectionModel::SelectCurrent);
    }

    Fdir_tmp = dir_tmp;

    for (int i = 0; i < Fid_files.length();i++) {
        QString name_file;
        name_file = QDir::toNativeSeparators(QString("%1/sm_%2.jpg").arg(Fdir_tmp).arg(Fid_files[i]));
        if (QFile::exists(name_file)) list_image_sm->item(i)->setIcon(QIcon(QPixmap(name_file)));
    }

    view_image(FcurrentImage);

    connect(list_image_sm,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));
}

PreviewCover::~PreviewCover()
{
    for (int i=0;i<list_image_sm->count();i++) {
        if (list_image_sm->item(i)->checkState()==Qt::Checked)
            Fcheck[i] = 0;
        else
            Fcheck[i] = 1;
        qDebug() << list_image_sm->item(i)->sizeHint();
    }
    emit check_stat(Fcheck);
}

void PreviewCover::on_pushButton_savefile_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save a cover"),
                                                     qApp->applicationDirPath(),
                                                     tr("jpg files (*.jpg)"));
    if (!filename.isEmpty()) {
        QString name_file;
        name_file = QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_tmp).arg(FcurrentImage));
        QFile::copy(name_file,filename);
    }
}

void PreviewCover::view_image(QString id_filename)
{
    if (id_filename == 0) {
        label_resolution->setText("");//tr("Preview cover"));
        QGraphicsScene *scene = new QGraphicsScene;
        cover_View->setScene(scene);

        cover_View->repaint();

        pushButton_savefile->setEnabled(false);
    } else {
        FcurrentImage = id_filename;

        QGraphicsScene *scene = new QGraphicsScene;
        QString filename = QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_tmp).arg(FcurrentImage));
        if (QFile::exists(filename)) {
            QPixmap pp;
            if (pp.load(filename)) {
                scene->addPixmap(pp);
                cover_View->setScene(scene);

                if ((cover_View->size().width()/pp.width())>(cover_View->size().height()/pp.height()))
                    scale = cover_View->size().height()/pp.height();
                else
                    scale = cover_View->size().width()/pp.width();

                if ((cover_View->sceneRect().right()!=0) & (cover_View->sceneRect().bottom()!=0)) {
                    if ((cover_View->size().width()/cover_View->sceneRect().right())>(cover_View->size().height()/cover_View->sceneRect().bottom()))
                        scale = cover_View->size().height()/cover_View->sceneRect().bottom();
                    else
                        scale = cover_View->size().width()/cover_View->sceneRect().right();
                }
                else scale = 1;
                label_resolution->setText(QString("%1x%2 (%3%)").arg(pp.width()).arg(pp.height()).arg(int(scale*100)));
                qDebug() << "scale: " << scale;

                QMatrix matr;
                matr.scale(scale,scale);
                cover_View->setMatrix(matr);

                pushButton_savefile->setEnabled(true);
            }
            else {
                qDebug() << "not load image";
                cover_View->setScene(scene);
                label_resolution->setText("");

                pushButton_savefile->setEnabled(false);
                emit download_image(FcurrentImage);
            }
        }
        else {
            qDebug() << "not exists file image";
            cover_View->setScene(scene);
            label_resolution->setText("");

            pushButton_savefile->setEnabled(false);
            emit download_image(FcurrentImage);
        }
    }
}

void PreviewCover::on_pushButton_zoom_out_clicked() {
    if (pushButton_savefile->isEnabled()) {
        QMatrix matr;
        if (scale>0.2)
            scale -= 0.1;
        QPixmap pix;
        if (pix.load(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_tmp).arg(FcurrentImage))))
            label_resolution->setText(QString("%1x%2 (%3%)").arg(pix.width()).arg(pix.height()).arg(int(scale*100)));
        else
            label_resolution->setText("");
        matr.scale(scale,scale);
        cover_View->setMatrix(matr);
        cover_View->repaint();
    }
}

void PreviewCover::on_pushButton_zoom_in_clicked() {
    if (pushButton_savefile->isEnabled()) {
        QMatrix matr;
        if (scale<3)
            scale +=0.1;
        QPixmap pix;
        if (pix.load(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_tmp).arg(FcurrentImage))))
            label_resolution->setText(QString("%1x%2 (%3%)").arg(pix.width()).arg(pix.height()).arg(int(scale*100)));
        else
            label_resolution->setText("");
        matr.scale(scale,scale);
        cover_View->setMatrix(matr);
        cover_View->repaint();
    }
}

void PreviewCover::itemClicked(QListWidgetItem *ret) {
    FcurrentImage = ret->text();
    view_image(FcurrentImage);
}

void PreviewCover::wheelEvent(QWheelEvent *event) {
    QListWidget *child = static_cast<QListWidget*>(childAt(event->pos()));
    if (!child)
        return;

    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    int tek = (-1);
    for(int i=0;i<list_image_sm->count();i++) {
        if (list_image_sm->item(i)==list_image_sm->currentItem()) {
            tek = i;
            break;
        }
    }
    numSteps = numSteps*(-1);
    if (numSteps>0) {
        if ((tek+numSteps)<list_image_sm->count())
            list_image_sm->setCurrentItem(list_image_sm->item(tek+numSteps));
        else
            list_image_sm->setCurrentItem(list_image_sm->item(list_image_sm->count()-1));
    }
    else {
        if ((tek+numSteps)>(-1))
            list_image_sm->setCurrentItem(list_image_sm->item(tek+numSteps));
        else
            list_image_sm->setCurrentItem(list_image_sm->item(0));
    }
    event->accept();
}

void PreviewCover::DownloadComplete(QString id) {
    if (id == FcurrentImage) view_image(FcurrentImage);
}
