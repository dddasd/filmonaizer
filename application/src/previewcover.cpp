#include "previewcover.h"
/*
PreviewCover::PreviewCover(bool pr,proxy_l pr_,QList<QString> name_files,QList<int> check,QString dir_tmp,int count_clicked, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    setModal(true);

    proxy = pr;
    proxy_list = pr_;

    zagr_img = "";

    im_download = false;
    pushButton_savefile->setEnabled(im_download);

    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

    QList<QByteArray> rr = QImageReader::supportedImageFormats();
    qDebug() << rr;
    int ii=0;
    for(int i=0;i<rr.length();i++)
        if ((rr[i]=="jpg") | (rr[i]=="jpeg")) {
            ii=1;
            break;
        }
    if (ii==0) {
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

    list_file = name_files;
    list_check = check;
    tek_img = count_clicked;


    list_image_sm->clear();
    list_image_sm->setViewMode(QListView::IconMode);
    list_image_sm->setFlow(QListView::TopToBottom);
    list_image_sm->setIconSize(QSize(128, 128));
    list_image_sm->scrollBarWidgets(Qt::AlignHorizontal_Mask);

    for(int i=0;i<list_file.length();i++) {
        QListWidgetItem *newAC = new QListWidgetItem(list_image_sm);
        newAC->setFlags(newAC->flags() | Qt::ItemIsUserCheckable);
        if (list_check[i]==0)
            newAC->setCheckState(Qt::Checked);
        else
            newAC->setCheckState(Qt::Unchecked);
        newAC->setText(list_file[i]);
        list_image_sm->addItem(newAC);
        if (tek_img==i)
            list_image_sm->setCurrentItem(newAC,QItemSelectionModel::SelectCurrent);
    }

    dir = dir_tmp;

    for (int i=0;i<list_file.length();i++) {
        if (!QFile::exists(dir+"/sm_"+list_file[i]+".jpg")) {
            list_image_sm->item(i)->setIcon(QIcon(":icons/question_image_min.png"));
            http_download *http = new http_download(this,"http://www.kinopoisk.ru/images/poster/sm_"+list_file[i]+".jpg",proxy,proxy_list.host,proxy_list.port,proxy_list.username,proxy_list.password);
            connect(http,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
            http->_download();
        }
        else
            list_image_sm->item(i)->setIcon(QIcon(QPixmap(dir+"/sm_"+list_file[i]+".jpg")));
    }

    if (QFile::exists(dir+"/"+list_file[tek_img]+".jpg")) {
        im_download = true;
        view_image(dir+"/"+list_file[tek_img]+".jpg");
    }
    else {
        view_image();
        http_download *http = new http_download(this,"http://www.kinopoisk.ru/images/poster/"+list_file[tek_img]+".jpg",proxy,proxy_list.host,proxy_list.port,proxy_list.username,proxy_list.password);
        connect(http,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
        http->_download();
    }
    connect(list_image_sm,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(_itemDoubleClicked(QListWidgetItem*)));
}

PreviewCover::~PreviewCover()
{
    for (int i=0;i<list_image_sm->count();i++) {
        if (list_image_sm->item(i)->checkState()==Qt::Checked)
            list_check[i] = 0;
        else
            list_check[i] = 1;
    }
    check_stat(list_check);
}

void PreviewCover::on_pushButton_savefile_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save a cover"),
                                                     qApp->applicationDirPath(),
                                                     tr("jpg files (*.jpg)"));
    if (!filename.isEmpty()) {
        QFile::copy(zagr_img,filename);
    }
}

void PreviewCover::view_image(QString filename)
{
    zagr_img = filename;
    if (filename.isEmpty()) {
        label_resolution->setText("");//tr("Preview cover"));
        QGraphicsScene *scene = new QGraphicsScene;
        cover_View->setScene(scene);

        cover_View->repaint();

        zagr_im = false;
    }
    else {        
        QGraphicsScene *scene = new QGraphicsScene;
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

                zagr_im = true;
            }
            else {
                qDebug() << "not load image";
                cover_View->setScene(scene);
                label_resolution->setText("");

                zagr_im = false;
            }
        }
        else {
            qDebug() << "not exists file image";
            cover_View->setScene(scene);
            label_resolution->setText("");

            zagr_im = false;
        }
    }
}

void PreviewCover::on_pushButton_zoom_out_clicked()
{
    if (zagr_im) {
        QMatrix matr;
        if (scale>0.2)
            scale -= 0.1;
        QPixmap pix;
        if (pix.load(zagr_img))
            label_resolution->setText(QString("%1x%2 (%3%)").arg(pix.width()).arg(pix.height()).arg(int(scale*100)));
        else
            label_resolution->setText("");
        matr.scale(scale,scale);
        cover_View->setMatrix(matr);
        cover_View->repaint();
    }
}

void PreviewCover::on_pushButton_zoom_in_clicked()
{
    if (zagr_im) {
        QMatrix matr;
        if (scale<3)
            scale +=0.1;
        QPixmap pix;
        if (pix.load(zagr_img))
            label_resolution->setText(QString("%1x%2 (%3%)").arg(pix.width()).arg(pix.height()).arg(int(scale*100)));
        else
            label_resolution->setText("");
        matr.scale(scale,scale);
        cover_View->setMatrix(matr);
        cover_View->repaint();
    }
}

void PreviewCover::fin_d(result_url ret_code)
{
    if (ret_code.code_r==0) {
        QRegExp reg_exp("sm_([\\d]*)");
        if (reg_exp.indexIn(ret_code.url)!=(-1)) {
            QFile file(dir+"/sm_"+reg_exp.cap(1)+".jpg");
            qDebug() << QString(dir+"/sm_"+reg_exp.cap(1)+".jpg");
            file.open(QIODevice::WriteOnly);
            file.write(ret_code.buf_d);
            file.close();

            for (int i=0;i<list_image_sm->count();i++) {
                if (list_image_sm->item(i)->text()==reg_exp.cap(1))
                    list_image_sm->item(i)->setIcon(QIcon(QPixmap(dir+"/sm_"+list_file[i]+".jpg")));
            }
        }
        else {
            QRegExp re("\\d+");
            if (re.indexIn(ret_code.url)!=(-1)) {
                QFile file(dir+"/"+re.cap(0)+".jpg");
                file.open(QIODevice::WriteOnly);
                file.write(ret_code.buf_d);
                file.close();
                if (list_file[tek_img]==re.cap(0)) {
                    im_download = true;
                    pushButton_savefile->setEnabled(im_download);
                    view_image(dir+"/"+re.cap(0)+".jpg");
                }
            }
        }
    }
}

void PreviewCover::_itemDoubleClicked(QListWidgetItem *ret)
{
    im_download = false;
    for (int i=0;i<list_image_sm->count();i++)
        if (list_image_sm->item(i)==ret)
            tek_img=i;
    if (QFile::exists(dir+"/"+ret->text()+".jpg")) {
        im_download = true;
        view_image(dir+"/"+ret->text()+".jpg");
    }
    else {
        view_image();
        http_download *http = new http_download(this,"http://www.kinopoisk.ru/images/poster/"+ret->text()+".jpg",proxy,proxy_list.host,proxy_list.port,proxy_list.username,proxy_list.password);
        connect(http,SIGNAL(fin_potok(result_url)),this,SLOT(fin_d(result_url)));
        http->_download();
    }
    pushButton_savefile->setEnabled(im_download);
}

void PreviewCover::wheelEvent(QWheelEvent *event)
{
    QListWidget *child = static_cast<QListWidget*>(childAt(event->pos()));
    if (!child)
        return;

    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    qDebug() << numSteps;

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
*/
