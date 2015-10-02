#include "previewdesc.h"

Previewdesc::Previewdesc(QString desc, QString link, QList<QString> templ, int current_templ, QString current_local, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setModal(true);

    menu_export = new QMenu;
    create_menu_export(current_local);

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

void Previewdesc::on_menu_export_triggered(QAction *act) {
    QDesktopServices::openUrl(QUrl(QString(act->data().toString()).arg(text_desc->toPlainText())));
}

void Previewdesc::create_menu_export(QString cur_local) {
    QFile file(QDir::toNativeSeparators(QApplication::applicationDirPath()+"/sendto.list"));

    if (!file.exists()) return;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QString icon = "",url = "",title = "";
    while (!file.atEnd()) {
        QString line(file.readLine());

        QRegExp rr("([^:]*)+:");

        if (rr.indexIn(line,0)!=(-1)) {
            line.remove(rr.cap(0));
            if (rr.cap(1) == "title") {
                QRegExp rr1("([^:]*)+:+([^,]*)+,");
                while (rr1.indexIn(line,0)!=(-1)) {
                    if (rr1.cap(1) == cur_local) {
                        title = rr1.cap(2);
                        break;
                    }
                    line.remove(rr1.cap(0));
                }
            }
            if (rr.cap(1) == "url") {
                if( line.endsWith("\n") ) line.truncate( line.length() - 1 );
                url = line;
            }
            if (rr.cap(1) == "icon") {
                if( line.endsWith("\n") ) line.truncate( line.length() - 1 );
                icon = line;
            }
        }

        if ((!icon.isEmpty()) && (!title.isEmpty()) && (!url.isEmpty())) {
            QPixmap pp;
            pp.loadFromData(QByteArray::fromBase64(icon.toUtf8()));
            QIcon icon_(pp);

            QAction *act_t = menu_export->addAction(icon_,title);
            act_t->setData(url);

            icon.clear();
            title.clear();
            url.clear();
        }
    }
    file.close();
    pushButton_export->setMenu(menu_export);
    connect(menu_export,SIGNAL(triggered(QAction*)),this,SLOT(on_menu_export_triggered(QAction*)));
}
