#include "previewdesc.h"

Previewdesc::Previewdesc(QString desc, QString link, QList<QString> templ, int current_templ, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setModal(true);

    link_kp = link;

    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);
    text_desc->setPlainText(QString::fromUtf8(desc.toUtf8()));

    comboBox_templates->addItems(templ);
    comboBox_templates->setCurrentIndex(current_templ);
    connect(comboBox_templates,SIGNAL(currentIndexChanged(QString)),this,SLOT(desc_change(QString)));
}

void Previewdesc::on_pushButton_close_clicked()
{
    close();
}

void Previewdesc::on_pushButton_save_file_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Select a save file"),
                                                 qApp->applicationDirPath(),
                                                 tr("All Text Files (*.txt)"));
    if (!filename.isEmpty()) {
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        file.write(text_desc->toPlainText().toUtf8());
        file.close();
    }
}

void Previewdesc::desc_change( QString templ )
{
    QString desc = template_change(templ);
    text_desc->setPlainText(QString::fromUtf8(desc.toUtf8()));
}

void Previewdesc::on_pushButton_save_buffer_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text_desc->toPlainText());
}

void Previewdesc::on_pushButton_save_twitter_clicked()
{
//http://twitter.com/home?status=
    QString ss(text_desc->toPlainText());
    if(ss.length()>140) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Twitter"));
        msgBox.setText(tr("Length of the tweet more than 140 characters. Trim message?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Yes: {
                ss.resize(140);
                //QDesktopServices::openUrl(QUrl(QString("http://twitter.com/home?status=%1").arg(ss)));
				QDesktopServices::openUrl(QUrl(QString("http://twitter.com/share?url=%1&via=filmonaizer&text=%2").arg(link_kp).arg(ss)));
                break;
            }
            case QMessageBox::No: {
                //QDesktopServices::openUrl(QUrl(QString("http://twitter.com/home?status=%1").arg(ss)));
				QDesktopServices::openUrl(QUrl(QString("http://twitter.com/share?url=%1&via=filmonaizer&text=%2").arg(link_kp).arg(ss)));
                break;
            }
        }
    }
    else
        QDesktopServices::openUrl(QUrl(QString("http://twitter.com/home?status=%1").arg(ss)));
}

void Previewdesc::on_pushButton_save_friendfeed_clicked()
{
    //http://frienfeed.com/?title=
    QDesktopServices::openUrl(QUrl(QString("http://friendfeed.com/?title=%1").arg(text_desc->toPlainText())));
}

void Previewdesc::on_pushButton_save_buzz_clicked()
{
    //http://www.google.com/buzz/post?title=
    QDesktopServices::openUrl(QUrl(QString("http://www.google.com/buzz/post?title=%1").arg(text_desc->toPlainText())));
}

void Previewdesc::on_pushButton_save_vkontakte_clicked()
{
    //http://vkontakte.ru/share.php?title=&url=
    QDesktopServices::openUrl(QUrl(QString("http://vkontakte.ru/share.php?url=%1vk/1/&title=%2").arg(link_kp).arg(text_desc->toPlainText())));
}

void Previewdesc::on_pushButton_save_facebook_clicked()
{
    //http://www.facebook.com/sharer.php?u=
    QString url(link_kp);
    url.append("fb/1/");
    QString url1(QString("http://www.facebook.com/sharer.php?u=%1").arg(url));//QString(QUrl::toPercentEncoding(url))));
    QDesktopServices::openUrl(QUrl(url1,QUrl::StrictMode));
}
