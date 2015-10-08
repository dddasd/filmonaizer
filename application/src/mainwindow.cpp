#include "mainwindow.h"

MainWindow::MainWindow(bool p, QWidget *parent): QMainWindow(parent) {
    setupUi(this);

    Ftags_global << "{;name_file}" << "{;ext_file}" << "{;cut_end}" << "{;codec}" << "{;path_save}"
                 << "{;save_to}" << "{;save_cover}" << "{;name_file_cover}" << "{;template_description}" << "{;custom}";

    if (!p) {
        Fdir_settings = QDir::toNativeSeparators(QDir::homePath()+"/.filmonaizer");
        Fdir_templates = QDir::toNativeSeparators(QDir::homePath()+"/.filmonaizer/Templates");
        Fdir_locale = QDir::toNativeSeparators(QDir::homePath()+"/.filmonaizer/translate");
        Fdir_tmp = QDir::toNativeSeparators(QDir::tempPath()+"/filmonaizer");
        Fdir_result = QDir::toNativeSeparators(QDir::homePath()+"/filmonaizer");
        Fdir_plugins = QDir::toNativeSeparators(QDir::homePath()+"/.filmonaizer/plugins");
    }
    else {
        Fdir_settings = QDir::toNativeSeparators(qApp->applicationDirPath());
        Fdir_templates = QDir::toNativeSeparators(qApp->applicationDirPath()+"/Templates");
        Fdir_locale = QDir::toNativeSeparators(qApp->applicationDirPath()+"/translate");
        Fdir_tmp = QDir::toNativeSeparators(qApp->applicationDirPath()+"/tmp");
        Fdir_result = QDir::toNativeSeparators(qApp->applicationDirPath()+"/result");
        Fdir_plugins = QDir::toNativeSeparators(qApp->applicationDirPath()+"/plugins");
    }

    if (!QDir(Fdir_settings).exists()) QDir("").mkdir(Fdir_settings);
    if (!QDir(Fdir_templates).exists()) QDir("").mkdir(Fdir_templates);
    if (!QDir(Fdir_locale).exists()) QDir("").mkdir(Fdir_locale);
    if (!QDir(Fdir_tmp).exists()) QDir("").mkdir(Fdir_tmp);
    if (!QDir(Fdir_result).exists()) QDir("").mkdir(Fdir_result);
    if (!QDir(Fdir_plugins).exists()) QDir("").mkdir(Fdir_plugins);

    menu_export = new QMenu;

    read_settings();

    //Load Plugins
    search_plugin();

    if (FlistPluginsSearch.isEmpty()) {
        QMessageBox msgBox(QMessageBox::Warning,tr("Fatal error"),tr("Not detected any plugins"),0,this);
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        this->setEnabled(false);
        Fcurrent_plugins_search = "";
        return;
    }

    bool plg = false;
    for (int i = 0; i < FlistPluginsSearch.count(); i++) {
        if (Fcurrent_plugins_search == FlistPluginsSearch[i].filename) {
            plg = true;
            break;
        }
    }
    if (!plg) Fcurrent_plugins_search = FlistPluginsSearch[0].filename;
    plg = load_plugin_search(QDir::toNativeSeparators(Fdir_plugins+"/"+Fcurrent_plugins_search));
    if (!plg) {
        for (int i = 0; i < FlistPluginsSearch.count(); i++) {
            plg = load_plugin_search(QDir::toNativeSeparators(Fdir_plugins+"/"+Fcurrent_plugins_search));
            if (plg) {
                Fcurrent_plugins_search = FlistPluginsSearch[i].filename;
                break;
            }
        }
        if (!plg) {
            QMessageBox msgBox(QMessageBox::Warning,tr("Fatal error"),tr("Not detected any plugins"),0,this);
            msgBox.addButton(QMessageBox::Ok);
            msgBox.exec();
            this->setEnabled(false);
            Fcurrent_plugins_search = "";
            return;
        }
    }

    if (FlistPluginsMovie.isEmpty()) {
        pushButton_movie_obzor->setEnabled(false);
        qDebug() << "not loaded plugin MediaFile";
    } else {
        plg = false;
        for (int i = 0; i < FlistPluginsMovie.count(); i++) {
            if (Fcurrent_plugins_movie == FlistPluginsMovie[i].filename) {
                plg = true;
                break;
            }
        }
        if (!plg) Fcurrent_plugins_movie = FlistPluginsMovie[0].filename;
        plg = load_plugin_movie(QDir::toNativeSeparators(Fdir_plugins+"/"+Fcurrent_plugins_movie));
        if (!plg) {
            for (int i = 0; i < FlistPluginsMovie.count(); i++) {
                plg = load_plugin_movie(QDir::toNativeSeparators(Fdir_plugins+"/"+Fcurrent_plugins_movie));
                if (plg) {
                    Fcurrent_plugins_movie = FlistPluginsMovie[i].filename;
                    break;
                }
            }
            if (!plg) {
                Fcurrent_plugins_movie = "";
                qDebug() << "not loaded plugin MediaFile";
            }
        }
        if (plg) pushButton_movie_obzor->setEnabled(true);
    }
    create_items_combobox_plug();
    connect(comboBox_plugins,SIGNAL(currentIndexChanged(int)),this,SLOT(comb_plug_indexChanged(int)));
    //Load Plgins end

    comboBox_history_movie->setLineEdit(searchLineEdit_movie);

    for (int i=0;i<Fhistory_search.count();i++) comboBox_history_movie->addItem(Fhistory_search[i]);
    comboBox_history_movie->setCurrentIndex(comboBox_history_movie->count()-1);

    statusbar_main->setSizeGripEnabled(false);

    button_expand = new QPushButton(statusbar_main);
    button_expand->setText("");
    button_expand->setFlat(true);
    button_expand->setMaximumSize(23,23);
    button_expand->setMinimumSize(23,23);
    button_expand->resize(23,23);
    button_expand->setToolTip(tr("Collapse"));
    button_expand->setIcon(QIcon(":icons/new/navigation-090-button.png"));
    connect(button_expand,SIGNAL(clicked()),this,SLOT(on_pushButton_expand_clicked()));
    statusbar_main->addWidget(button_expand);

    label_zap = new QLabel("",statusbar_main);
    statusbar_main->addWidget(label_zap,1);

    label_icon = new QLabel("",statusbar_main);
    label_icon->setText("<img src="":icons/new/tick-button.png"" />");
    label_icon->setTextFormat(Qt::RichText);
    label_icon->setMaximumSize(23,23);
    label_icon->setMinimumSize(23,23);
    label_icon->resize(QSize(23,23));
    label_icon->setToolTip("");
    statusbar_main->addWidget(label_icon);

    movie = new QMovie(":/icons/loading.gif");
    connect(movie,SIGNAL(frameChanged(int)),this,SLOT(setItemIcon(int)));
    if (movie->loopCount() != -1) connect(movie,SIGNAL(finished()),movie,SLOT(start()));

    expand_s = true;

    lineEdit_dir_obzor->setText(Fdir_result);

    if (FtemplateCheck) checkBox_save_templates->setCheckState(Qt::Checked);
    if (FrewriteFile) checkBox_rewite_file->setCheckState(Qt::Checked);

    FparsCommand = (-1);

    FimageDownload.clear();
    bool_fr_pr_image = false;

    translation(Fcurrent_locale);
}

MainWindow::~MainWindow() {
    FtemplateCheck = (checkBox_save_templates->checkState()==Qt::Checked);

    FrewriteFile = (checkBox_rewite_file->checkState()==Qt::Checked);

    write_settings();

    if (Fclear_tmp_exit) {
        QDir dir = Fdir_tmp;
        QStringList name_filters;
        name_filters << "*.*";
        QStringList fileNames = dir.entryList(name_filters,QDir::Files);
        for (int i=0;i<fileNames.length();i++) QFile::remove(dir.absolutePath()+QDir::separator()+fileNames[i]);
    }
}

void MainWindow::slotSearch(QList<QString> ll,int ii, QString er) {
    if (ii==0) {
        label_icon->setText("<img src="":icons/new/tick-button.png"" />");
        label_icon->setToolTip("");

        disconnect(treeWidget_search_result,SIGNAL(itemExpanded(QTreeWidgetItem*)),this,SLOT(itemExpanded(QTreeWidgetItem*)));
        disconnect(treeWidget_search_result,SIGNAL(itemCollapsed(QTreeWidgetItem*)),this,SLOT(itemCollapsed(QTreeWidgetItem*)));
        disconnect(treeWidget_search_result,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));

        film_item.clear();
        desc_item.clear();
        image_item.clear();

        treeWidget_search_result->clear();
        treeWidget_search_result->setColumnCount(1);
        treeWidget_search_result->setHeaderHidden(true);

        for (int i = 0; i < ll.count(); i++) {
            film_item.append(new QTreeWidgetItem(treeWidget_search_result));
            film_item[film_item.length()-1]->setText(0, ll[i]);
            film_item[film_item.length()-1]->setIcon(0,QIcon(":icons/folder-horizontal.png"));

            desc_item.append(new QTreeWidgetItem(film_item[film_item.length()-1]));
            desc_item[desc_item.length()-1]->setText(0, tr("Description movie"));
            desc_item[desc_item.length()-1]->setIcon(0,QIcon(":icons/sticky-note-text.png"));

            image_item.append(new QTreeWidgetItem(film_item[film_item.length()-1]));
            image_item[image_item.length()-1]->setText(0, tr("Covers"));
            image_item[image_item.length()-1]->setIcon(0,QIcon(":icons/image.png"));

            treeWidget_search_result->insertTopLevelItem(0,film_item[film_item.length()-1]);
        }

        connect(treeWidget_search_result,SIGNAL(itemExpanded(QTreeWidgetItem*)),this,SLOT(itemExpanded(QTreeWidgetItem*)));
        connect(treeWidget_search_result,SIGNAL(itemCollapsed(QTreeWidgetItem*)),this,SLOT(itemCollapsed(QTreeWidgetItem*)));
        connect(treeWidget_search_result,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
    } else {
        label_icon->setText("<img src="":icons/new/minus-button.png"" />");
        label_icon->setToolTip(er);
    }
}

void MainWindow::on_pushButton_search_clicked() {
    if(!searchLineEdit_movie->text().isEmpty()) {
        for (int i=0;i<Fhistory_search.length();i++)
            if (searchLineEdit_movie->text()==Fhistory_search[i]) {
                Fhistory_search.removeAt(i);
                break;
            }

        Fhistory_search.append(searchLineEdit_movie->text());
        while (Fhistory_search.length()>10) {
                Fhistory_search.removeFirst();
        }
        comboBox_history_movie->clear();
        for (int i=0;i<Fhistory_search.length();i++)
                comboBox_history_movie->addItem(Fhistory_search[i]);
        comboBox_history_movie->setCurrentIndex(comboBox_history_movie->count()-1);

        if ((Fproxy) && (Fproxy_list.count() > 0)) {
            if ((Fcurrent_proxy < 0) || (Fcurrent_proxy >= Fproxy_list.count())) {
                Fcurrent_proxy = 0;
            }
            plugin_search->set_proxy(Fproxy,Fproxy_list[Fcurrent_proxy].host,Fproxy_list[Fcurrent_proxy].port,
                                Fproxy_list[Fcurrent_proxy].username,Fproxy_list[Fcurrent_proxy].password);
        } else {
            plugin_search->set_proxy(false,"","","","");
        }
        plugin_search->result_search_movie(searchLineEdit_movie->text());
    }
}

bool MainWindow::load_plugin_search(QString fileName) {
    treeWidget_search_result->clear();
    FimageDownload.clear();
    FimageDownloadDone.clear();
    if (!QFile(fileName).exists()) return false;
    if (pluginLoaderSearch.isLoaded()) pluginLoaderSearch.unload();
    pluginLoaderSearch.setFileName(fileName);
    QObject *plugin = pluginLoaderSearch.instance();
    if (plugin) {
        plugin_search = qobject_cast<QInterfacePluginSearch *>(plugin);
        if (!plugin_search) return false;
        else {
            plugin_search->init_plug();
            QObject *ttt = plugin_search->notifyer();
            connect(ttt,SIGNAL(signalSearch(QList<QString>,int,QString)),this,SLOT(slotSearch(QList<QString>,int,QString)));
            connect(ttt,SIGNAL(signalPars(int,QString)),this,SLOT(slotPars(int,QString)));
            connect(ttt,SIGNAL(signalSmallImage(int,QList<QString>)),this,SLOT(slotSmallImage(int,QList<QString>)));
            connect(ttt,SIGNAL(signalDownloadImage(QString,int)),this,SLOT(slotDownloadImage(QString,int)));
            Ftags_plug.clear();
            Ftags_plug = plugin_search->listTags();
            qDebug() << Ftags_plug;
            return true;
        }
    } else {
        qDebug() << pluginLoaderSearch.errorString();
        return false;
    }
}

bool MainWindow::load_plugin_movie(QString fileName) {
    if (!QFile(fileName).exists()) return false;
    if (pluginLoaderMovie.isLoaded()) pluginLoaderMovie.unload();
    pluginLoaderMovie.setFileName(fileName);
    QObject *plugin = pluginLoaderMovie.instance();
    if (plugin) {
        plugin_movie = qobject_cast<QInterfacePluginMovie *>(plugin);
        if (!plugin_movie) return false;
        else {
            plugin_movie->init_plug();
            Ftags_mediafile.clear();
            Ftags_mediafile = plugin_movie->listTags();
            qDebug() << Ftags_mediafile;
            return true;
        }
    } else {
        qDebug() << pluginLoaderMovie.errorString();
        return false;
    }
}

void MainWindow::slotPars(int ii,QString err) {
    switch (FparsCommand) {
        case 0: //Превью окно
            movie->stop();
            for (int i = 0; i < desc_item.count(); i++) {
                desc_item[i]->setIcon(0,QIcon(":icons/sticky-note-text.png"));
            }
            if (ii == 0) {
                if (treeWidget_templates->topLevelItemCount() == 0) {
                    QMessageBox msgBox;
                    msgBox.setText(tr("Not search a template!"));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.exec();
                    return;
                }

                QList<QString> templ;
                int j = (-1);
                for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
                    templ.append(treeWidget_templates->topLevelItem(i)->text(0));
                    if (j==(-1)) {
                        if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {
                            j = i;
                        }
                    }
                }

                if (j==(-1)) j = 0;

                QString text_otb = "";
                QString FnameFile = "";
                QString FextFile = "";
                QString Fcodec = "";
                QString FpathSave = "";
                QString FnameFileCover = "";
                int FsaveTo = (-1);
                int FsaveCover = (-1);

                text_otb = pars_template(Fdir_templates+"/"+templ[j],FnameFile,FextFile,Fcodec,FpathSave,FnameFileCover,FsaveTo,FsaveCover);

                Previewdesc *prev = new Previewdesc(text_otb,"",templ,j,Fcurrent_locale);
                prev->setAttribute(Qt::WA_DeleteOnClose);
                prev->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
                connect(prev,SIGNAL(template_change(QString)),this,SLOT(template_change(QString)));
                prev->show();
            } else {
                qDebug() << tr("error slotPars - ") << err;
            }
            break;
        case 1: //Сохранить в файл
            save_description(false);
            label_icon->setText("<img src="":icons/new/tick-button.png"" />");
            label_icon->setToolTip("");
            break;
        case 2: //Сохранить в буфер
            save_description(true);
            label_icon->setText("<img src="":icons/new/tick-button.png"" />");
            label_icon->setToolTip("");
            break;
        case 3: //OpenUrl
            if (ii == 0) {
                if (treeWidget_templates->topLevelItemCount() == 0) {
                    QMessageBox msgBox;
                    msgBox.setText(tr("Not search a template!"));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.exec();
                    return;
                }

                QList<QString> templ;
                int j = (-1);
                for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
                    templ.append(treeWidget_templates->topLevelItem(i)->text(0));
                    if (j==(-1)) {
                        if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {
                            j = i;
                        }
                    }
                }

                if (j==(-1)) j = 0;

                QString text_otb = "";
                QString FnameFile = "";
                QString FextFile = "";
                QString Fcodec = "";
                QString FpathSave = "";
                QString FnameFileCover = "";
                int FsaveTo = (-1);
                int FsaveCover = (-1);

                text_otb = pars_template(Fdir_templates+"/"+templ[j],FnameFile,FextFile,Fcodec,FpathSave,FnameFileCover,FsaveTo,FsaveCover);

                QDesktopServices::openUrl(QUrl(QString(FopenUrl).arg(text_otb)));
            }
            break;
    }
    FparsCommand = (-1);
}

void MainWindow::slotSmallImage(int err, QList<QString> list) {
    movie->stop();
    image_item[FsmallImageClick]->setIcon(0,QIcon(":icons/image.png"));

    if (err != 0) {
        for (int i = 0; i < list.count(); i++) qDebug() << list[i];
        label_icon->setText("<img src="":icons/new/minus-button.png"" />");
        label_icon->setToolTip(tr("error small image"));
        image_item[FsmallImageClick]->setIcon(0,QIcon(":icons/image.png"));
        FsmallImageClick = (-1);
        return;
    }
    if ((FsmallImageClick < 0) || (image_item.count() < FsmallImageClick)) return;
    for (int i = 0; i < list.count(); i++) {
        QTreeWidgetItem *new_item = new QTreeWidgetItem(image_item[FsmallImageClick]);
        new_item->setText(0, list[i]);
        new_item->setFlags(new_item->flags()|Qt::ItemIsUserCheckable);
        new_item->setCheckState(0,Qt::Unchecked);
        treeWidget_search_result->insertTopLevelItem(0,new_item);
    }

    if (!FSavingModeTraffic) plugin_search->download_all_image(Fdir_tmp);

    image_item[FsmallImageClick]->setExpanded(true);
    label_icon->setText("<img src="":icons/new/tick-button.png"" />");
    label_icon->setToolTip("");

    FsmallImageClick = (-1);
}

void MainWindow::slotDownloadImage(QString id,int err) {
    Q_UNUSED(err);
    if (bool_fr_pr_image) {
        form_pr_image->DownloadComplete(id);
    }
    if (!FimageDownload.isEmpty()) {
        if (FimageDownload.removeOne(QString("%1").arg(id))) {
            FimageDownloadDone << QString("%1").arg(id);
        }
        if (FimageDownload.isEmpty()) {
            FparsCommand = 1;
            plugin_search->result_pars_movie(FCountMovie,Fdir_tmp);
        }
    }
}

QString MainWindow::template_change(QString templ) {
    QString text_otb = "";
    QString FnameFile = "";
    QString FextFile = "";
    QString Fcodec = "";
    QString FpathSave = "";
    QString FnameFileCover = "";
    int FsaveTo = (-1);
    int FsaveCover = (-1);

    text_otb = pars_template(Fdir_templates+"/"+templ,FnameFile,FextFile,Fcodec,FpathSave,FnameFileCover,FsaveTo,FsaveCover);

    return text_otb;
}

void MainWindow::setItemIcon(int) {
    icon_ch->setIcon(0,movie->currentPixmap());
}

void MainWindow::itemExpanded(QTreeWidgetItem* ret) {
    for (int i=0;i<film_item.length();i++)
        if (ret==film_item[i]) {
            ret->setIcon(0,QIcon(":icons/folder-horizontal-open.png"));
            break;
        }
}

void MainWindow::itemCollapsed(QTreeWidgetItem* ret) {
    for (int i=0;i<film_item.length();i++)
        if (ret==film_item[i]) {
            ret->setIcon(0,QIcon(":icons/folder-horizontal.png"));
            break;
        }
}

void MainWindow::itemDoubleClicked(QTreeWidgetItem* ret,int col) {
    Q_UNUSED(col);
    for (int i=0;i<film_item.length();i++)
        if (ret==film_item[i])
            return;
    for (int i=0;i<desc_item.length();i++)
        if (ret==desc_item[i]) {            
            FparsCommand = 0;
            icon_ch = ret;
            movie->start();
            plugin_search->result_pars_movie(i,Fdir_tmp);
            return;
        }
    for (int i=0;i<image_item.length();i++)
        if (ret==image_item[i]) {
            if (ret->childCount() > 0) return;
            FsmallImageClick = i;
            icon_ch = ret;
            movie->start();
            plugin_search->result_search_small_image(i,Fdir_tmp);
            return;
        }

    QList<QString> id_files;
    QList<int> check_files;

    for (int i=0;i<ret->parent()->childCount();i++) {
        id_files << ret->parent()->child(i)->text(0);
        if (ret->parent()->child(i)->checkState(0)==Qt::Checked)
            check_files << 0;
        else
            check_files << 1;
    }

    form_pr_image = new PreviewCover(id_files,check_files,Fdir_tmp,ret->text(0),this);
    form_pr_image->setAttribute(Qt::WA_DeleteOnClose);
    form_pr_image->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    connect(form_pr_image,SIGNAL(download_image(QString)),this,SLOT(slot_form_download_image(QString)));
    connect(form_pr_image,SIGNAL(check_stat(QList<int>)),this,SLOT(check_stat(QList<int>)));
    connect(form_pr_image,SIGNAL(finished(int)),this,SLOT(slot_form_close()));
    plugin_search->download_image(ret->text(0),Fdir_tmp);
    form_pr_image->show();
    bool_fr_pr_image = true;
}

void MainWindow::on_pushButton_expand_clicked() {
    expanded(expand_s);
}

void MainWindow::expanded(bool ex) {
    if(ex) {
        expand = this->height() - frame_expanded->height() - widget_filmonaizer->layout()->spacing();
        collapse = this->height();

        frame_expanded->setVisible(false);

        this->setMinimumSize(this->width(),expand);
        this->setMaximumSize(this->width(),expand);
        this->resize(this->width(),expand);

        button_expand->setIcon(QIcon(":icons/new/navigation-270-button.png"));
        button_expand->setToolTip(tr("Expand"));
        expand_s = false;
    }
    else {
        frame_expanded->setVisible(true);

        this->setMinimumSize(this->width(),collapse);
        this->setMaximumSize(this->width(),collapse);
        this->resize(this->width(),collapse);

        button_expand->setIcon(QIcon(":icons/new/navigation-090-button.png"));
        button_expand->setToolTip(tr("Collapse"));
        expand_s = true;
    }
}

void MainWindow::on_pushButton_about_clicked() {
    AboutDialog *ab = new AboutDialog(Fcurrent_locale);
    ab->setAttribute(Qt::WA_DeleteOnClose);
    ab->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    connect(ab,SIGNAL(ch_locale(QString)),this,SLOT(translation(QString)));
    ab->show();
}

void MainWindow::read_settings() {
    QSettings settings(QDir::toNativeSeparators(Fdir_settings+"/settings.ini"),QSettings::IniFormat,0);

    int size = settings.beginReadArray("proxy");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        struct_proxy_list proxy_list;
        proxy_list.host = settings.value("host").toString();
        proxy_list.port = settings.value("port").toString();
        proxy_list.username = settings.value("username").toString();
        proxy_list.password = settings.value("password").toString();
        Fproxy_list.append(proxy_list);
    }
    settings.endArray();

    size = settings.beginReadArray("history");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        Fhistory_search.append(settings.value("string").toString());
    }
    settings.endArray();

    QDir dir = Fdir_templates;
    QStringList name_filters;
    name_filters << "*.txt";
    QStringList fileNames = dir.entryList(name_filters,QDir::Files);
    //create_item_tree_templ("default",false);
    for (int i = 0; i < fileNames.count(); i++) create_item_tree_templ(fileNames[i],false);
    size = settings.beginReadArray("template");
    for (int i=0;i<size;i++) {
        settings.setArrayIndex(i);
        QString name = settings.value("name","").toString();
        bool checked = settings.value("checked",false).toBool();
        for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
            if (name == treeWidget_templates->topLevelItem(i)->text(0)) {
                if (checked) treeWidget_templates->topLevelItem(i)->setCheckState(0,Qt::Checked);
            }
        }
    }
    settings.endArray();

    settings.beginGroup("main");
    Fcurrent_locale = settings.value("locale","ru_RU").toString();
    Fclear_tmp_exit = settings.value("clear_tmp_exit",true).toBool();
    Fcurrent_proxy = settings.value("current_proxy",(-1)).toInt();
    Fproxy = settings.value("enable_proxy",false).toBool();
    FtemplateCheck = settings.value("get_template",false).toBool();
    FrewriteFile = settings.value("rewrite_a_file",false).toBool();
    Fcurrent_plugins_movie = settings.value("plugin_movie","").toString();
    Fcurrent_plugins_search = settings.value("plugin_search","").toString();
    FSavingModeTraffic = settings.value("saving_mode_traffic",false).toBool();
    settings.endGroup();
}

void MainWindow::write_settings() {
    QSettings settings(QDir::toNativeSeparators(Fdir_settings+"/settings.ini"),QSettings::IniFormat,0);

    settings.remove("proxy");
    settings.beginWriteArray("proxy");
    for (int i = 0; i < Fproxy_list.count(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("host", Fproxy_list.at(i).host);
        settings.setValue("port", Fproxy_list.at(i).port);
        settings.setValue("username", Fproxy_list.at(i).username);
        settings.setValue("password", Fproxy_list.at(i).password);
    }
    settings.endArray();

    settings.remove("history");
    settings.beginWriteArray("history");
    for (int i = 0; i < Fhistory_search.count(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("string", Fhistory_search.at(i));
    }
    settings.endArray();

    settings.remove("template");
    settings.beginWriteArray("template");
    for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
        settings.setArrayIndex(i);
        settings.setValue("name",treeWidget_templates->topLevelItem(i)->text(0));
        if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {
            settings.setValue("checked",true);
        } else {
            settings.setValue("checked",false);
        }
    }
    settings.endArray();

    settings.remove("main");
    settings.beginGroup("main");
    settings.setValue("locale",Fcurrent_locale);
    settings.setValue("clear_tmp_exit",Fclear_tmp_exit);
    settings.setValue("current_proxy",Fcurrent_proxy);
    settings.setValue("enable_proxy",Fproxy);
    settings.setValue("get_template",FtemplateCheck);
    settings.setValue("rewrite_a_file",FrewriteFile);
    settings.setValue("plugin_movie",Fcurrent_plugins_movie);
    settings.setValue("plugin_search",Fcurrent_plugins_search);
    settings.setValue("saving_mode_traffic",FSavingModeTraffic);
    settings.endGroup();

    settings.sync();
}

void MainWindow::create_item_tree_templ(QString name, bool ch) {
    treeWidget_templates->setColumnCount(1);
    treeWidget_templates->setHeaderHidden(true);

    QTreeWidgetItem *new_item = new QTreeWidgetItem(treeWidget_templates);
    new_item->setText(0, name);
    new_item->setFlags(new_item->flags()|Qt::ItemIsUserCheckable);
    if (ch)
        new_item->setCheckState(0,Qt::Checked);
    else
        new_item->setCheckState(0,Qt::Unchecked);
    new_item->setToolTip(0,read_template_description(QDir::toNativeSeparators(Fdir_templates+"/"+name)));

    treeWidget_templates->insertTopLevelItem(0,new_item);
}

void MainWindow::translation(QString i) {
    qDebug() << i;
    if (i == "ru_RU") {
        if (trans.load(Fdir_locale+QDir::separator()+"filmonaizer_ru_RU")) {
            qApp->installTranslator(&trans);
            retranslateUi(this);
            Fcurrent_locale = i;
            create_menu_export();
            disconnect(comboBox_plugins,SIGNAL(currentIndexChanged(int)),this,SLOT(comb_plug_indexChanged(int)));
            for (int i = 0; i < FlistPluginsSearch.count(); i++)
                read_list_plugin(QDir::toNativeSeparators(QString("%1/%2.list").arg(Fdir_plugins).arg(FlistPluginsSearch[i].filename)),&FlistPluginsSearch[i]);
            create_items_combobox_plug();
            connect(comboBox_plugins,SIGNAL(currentIndexChanged(int)),this,SLOT(comb_plug_indexChanged(int)));
            return;
        }
    }
    qApp->removeTranslator(&trans);
    retranslateUi(this);
    Fcurrent_locale = "en_US";
    create_menu_export();
    disconnect(comboBox_plugins,SIGNAL(currentIndexChanged(int)),this,SLOT(comb_plug_indexChanged(int)));
    for (int i = 0; i < FlistPluginsSearch.count(); i++)
        read_list_plugin(QDir::toNativeSeparators(QString("%1/%2.list").arg(Fdir_plugins).arg(FlistPluginsSearch[i].filename)),&FlistPluginsSearch[i]);
    create_items_combobox_plug();
    connect(comboBox_plugins,SIGNAL(currentIndexChanged(int)),this,SLOT(comb_plug_indexChanged(int)));
}

void MainWindow::search_plugin() {
    QDir dir = Fdir_plugins;
    QStringList name_filters;
#ifdef Q_OS_WIN
    name_filters << "*.dll";
#else
    name_filters << "*.so";
#endif
    QStringList fileNamePlugins = dir.entryList(name_filters,QDir::Files);

    FlistPluginsMovie.clear();
    FlistPluginsSearch.clear();
    for (int i = 0; i < fileNamePlugins.count(); i++) {
        QString ff = QDir::toNativeSeparators(QString("%1/%2").arg(Fdir_plugins).arg(fileNamePlugins[i]));

        //Search
        if (pluginLoaderSearch.isLoaded()) pluginLoaderSearch.unload();
        pluginLoaderSearch.setFileName(ff);
        QObject *plugin = pluginLoaderSearch.instance();
        if (plugin) {
            plugin_search = qobject_cast<QInterfacePluginSearch *>(plugin);
            if (plugin_search) {
                struct_plugins_list lp;
                lp.filename = fileNamePlugins[i];
                lp.version = plugin_search->getVersion();
                read_list_plugin(QString("%1.list").arg(ff),&lp);
                FlistPluginsSearch << lp;
                continue;
            }
        }

        //Movie
        if (pluginLoaderMovie.isLoaded()) pluginLoaderMovie.unload();
        pluginLoaderMovie.setFileName(ff);
        plugin = pluginLoaderMovie.instance();
        if (plugin) {
            plugin_movie = qobject_cast<QInterfacePluginMovie *>(plugin);
            if (plugin_movie) {
                struct_plugins_list lp;
                lp.filename = fileNamePlugins[i];
                lp.version = plugin_movie->getVersion();
                read_list_plugin(QString("%1.list").arg(ff),&lp);
                FlistPluginsMovie << lp;
                continue;
            }
        }
    }

    qDebug() << fileNamePlugins;
    //qDebug() << Ffilename_plugin_search;
    //qDebug() << Ffilename_plugin_movie;
}

void MainWindow::read_list_plugin(QString filename, struct_plugins_list *pl) {
    QFile file(filename);

    if (!file.exists()) return;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    while (!file.atEnd()) {
        QString line(file.readLine());

        QRegExp rr("([^:]*)+:");

        if (rr.indexIn(line,0)!=(-1)) {
            line.remove(rr.cap(0));
            if (rr.cap(1) == "description") {
                QRegExp rr1("([^:]*)+:+([^,]*)+,");
                while (rr1.indexIn(line,0)!=(-1)) {
                    if (rr1.cap(1) == Fcurrent_locale) {
                        pl->description = rr1.cap(2);
                        break;
                    }
                    line.remove(rr1.cap(0));
                }
            }
            if (rr.cap(1) == "title") {
                QRegExp rr1("([^:]*)+:+([^,]*)+,");
                while (rr1.indexIn(line,0)!=(-1)) {
                    if (rr1.cap(1) == Fcurrent_locale) {
                        pl->title = rr1.cap(2);
                        break;
                    }
                    line.remove(rr1.cap(0));
                }
            }
            if (rr.cap(1) == "icon") {
                if( line.endsWith("\n") ) line.truncate( line.length() - 1 );
                pl->icon = QByteArray::fromBase64(line.toUtf8());
            }
        }
    }

    file.close();
}

QString MainWindow::pars_template(QString file_name,
                                  QString &FSetNameFile,
                                  QString &FSetExtFile,
                                  QString &FSetCodec,
                                  QString &FSetPathSave,
                                  QString &FSetNameFileCover,
                                  int &FSetSaveTo,
                                  int &FSetSaveCover) {
    /*
    Global Settings:
    {;name_file}		имя файла
    {;ext_file}			расширение файла
    {;cut_end}			окончание строки если превышает заданный размер
    {;codec}			кодировка в которой сохранять
    {;path_save}		путь до директории, в которую сохраняем файлы
    {;save_to}			вариант сохранения: 0 - в буфер, 1 - в файл, 2 - в оба места
    {;save_cover}		сохранять обложки или нет: 0 - сохранять, 1 - не сохранять
    {;name_file_cover}	имя файла обложки

    {;custom}			пользовательские переменные (не реализовано)
    {;use_short_url}	использовать сервер коротких ссылок (отключено) (не реализовано)*/

    QFile file_s;
    file_s.setFileName(QDir::toNativeSeparators(file_name));
    qDebug() << file_name;

    if (!file_s.exists()) {
        qDebug() << QString("%1 - %2").arg(tr("not exists file template")).arg(file_name);
        return "";
    }

    if (!file_s.open(QIODevice::ReadOnly)) {
        qDebug() << QString("%1 - %2").arg(tr("not open file template")).arg(file_name);
        return "";
    }

    QString file_shab = QString::fromUtf8(file_s.readAll());
    file_s.close();

    QString FnameFile = "";
    QString FextFile = "";
    QString FcutEnd = "";
    QString Fcodec = "";
    QString FpathSave = "";
    QString FnameFileCover = "";
    int FsaveTo = (-1);
    int FsaveCover = (-1);

    //Search global tegs
    QList<list_variables> Flist_variables;
    QRegExp reg_exp("\\{;([^=]*)=([^}]*)\\}+(.*)\\n");
    reg_exp.setMinimal(true);
    int pos_r = reg_exp.indexIn(file_shab,0);
    while (pos_r!=(-1)) {
        if (reg_exp.cap(1)=="name_file") {
            if (reg_exp.cap(2).indexOf("\"")!=(-1)) {
                FnameFile = reg_exp.cap(2).replace("\"","");
            } else {
                FnameFile = plugin_search->result_tags(QString("{:%1}").arg(reg_exp.cap(2)));
            }
        }

        if (reg_exp.cap(1)=="name_file_cover") {
            if (reg_exp.cap(2).indexOf("\"")!=(-1)) {
                FnameFileCover = reg_exp.cap(2).replace("\"","");
            } else {
                FnameFileCover = plugin_search->result_tags(QString("{:%1}").arg(reg_exp.cap(2)));
            }
        }

        if (reg_exp.cap(1)=="ext_file") {
            FextFile = reg_exp.cap(2);
        }

        if (reg_exp.cap(1)=="cut_end") {
            FcutEnd = reg_exp.cap(2).replace("\"","");
        }

        if (reg_exp.cap(1)=="codec") {
            Fcodec = reg_exp.cap(2).replace("\"","");
        }

        if (reg_exp.cap(1)=="path_save") {
            FpathSave = reg_exp.cap(2).replace("\"","");
        }

        if (reg_exp.cap(1)=="save_to") {
            bool b = false;
            FsaveTo = reg_exp.cap(2).toInt(&b);
            if (!b) FsaveTo = (-1);
        }

        if (reg_exp.cap(1)=="save_cover") {
            bool b = false;
            FsaveCover = reg_exp.cap(2).toInt(&b);
            if (!b) FsaveCover = (-1);
        }

        QRegExp reg_exp_tmp("custom([^=]*)=\\(name=\"([^\"]*)\"\\)([^\\}]*)+\\}");
        if (reg_exp_tmp.indexIn(reg_exp.cap(0),0)!=(-1)) {
            list_variables list_v_temp;
            list_v_temp.tag = QString("custom%1").arg(reg_exp_tmp.cap(1));
            list_v_temp.name_v = reg_exp_tmp.cap(2);
            QRegExp reg_exp_tmp2("\\(item=\"([^\"]*)\"\\)");
            int j = reg_exp_tmp2.indexIn(reg_exp_tmp.cap(3),0);
            while(j!=(-1)) {
                list_v_temp.value_v.append(reg_exp_tmp2.cap(1));
                j = reg_exp_tmp2.indexIn(reg_exp_tmp.cap(3),j+1);
            }

            QRegExp reg_exp_tmp3(QString("\\{:%1\\}").arg(list_v_temp.tag));
            if (reg_exp_tmp3.indexIn(file_shab,0)!=(-1)) {
                Flist_variables << list_v_temp;
            }
        }

        file_shab.replace(reg_exp.cap(0),"");
        pos_r = reg_exp.indexIn(file_shab,0);
    }
    //\Search global tegs

    if (Flist_variables.count() > 0) {
        dialog_variables *dial = new dialog_variables(&Flist_variables,this);
        dial->setAttribute(Qt::WA_DeleteOnClose);
        dial->exec();

        for (int i = 0; i < Flist_variables.count(); i++) {
            reg_exp.setPattern(QString("\\{:%1\\}").arg(Flist_variables[i].tag));
            if (reg_exp.indexIn(file_shab,0)!=(-1)) {
                file_shab.replace(reg_exp.cap(0),Flist_variables[i].value_s);
            }
        }
        Flist_variables.clear();
    }

    //
    reg_exp.setPattern("\\{:([^\\}]*)+\\}");
    pos_r = reg_exp.indexIn(file_shab,0);
    while(pos_r!=(-1)) {
        QRegExp reg_exp_temp("([^\\(]*)+\\(");
        if (reg_exp_temp.indexIn(reg_exp.cap(1),0)!=(-1)) { //есть доппараметры
            QString razd = "";
            int length_m = (-1);
            reg_exp_temp.setPattern("\\(+([^=]*)+\\=+([^\\)]*)+\\)");
            QString temp_str = reg_exp.cap(0);
            int pos_t = reg_exp_temp.indexIn(temp_str,0);
            while (pos_t!=(-1)) {
                if (reg_exp_temp.cap(1)=="i") {
                    razd = reg_exp_temp.cap(2);
                }
                if (reg_exp_temp.cap(1)=="l") {
                    length_m = reg_exp_temp.cap(2).toInt();
                    if (length_m <= 0) length_m = (-1);
                }
                temp_str.replace(reg_exp_temp.cap(0),"");
                pos_t = reg_exp_temp.indexIn(temp_str,0);
            }

            QString str_r = plugin_search->result_tags(temp_str);
            if (str_r.isEmpty()) str_r = plugin_movie->result_tags(temp_str);
            if (!razd.isEmpty()) str_r.replace(",",razd);
            if (length_m!=(-1)) {
                if (str_r.length()>length_m) {
                    str_r = str_r.mid(0,length_m)+FcutEnd;
                }
            }
            file_shab.replace(reg_exp.cap(0),str_r);
        } else {
            QString str_r = plugin_search->result_tags(reg_exp.cap(0));
            if (str_r.isEmpty()) str_r = plugin_movie->result_tags(reg_exp.cap(0));
            file_shab.replace(reg_exp.cap(0),str_r);
        }
        pos_r = reg_exp.indexIn(file_shab,0);
    }

    FSetNameFile = FnameFile;
    FSetExtFile = FextFile;
    FSetCodec = Fcodec;
    FSetPathSave = FpathSave;
    FSetNameFileCover = FnameFileCover;
    FSetSaveTo = FsaveTo;
    FSetSaveCover = FsaveCover;
    return file_shab;
}


void MainWindow::on_pushButton_savefile_clicked() {
    if ((treeWidget_search_result->topLevelItemCount() > 0) && (film_item.length() > 0) && (treeWidget_search_result->currentItem() != NULL)) {
        label_icon->setText("<img src="":icons/new/information-button.png"" />");

        FCountMovie = (-1);

        for (int i = 0; i < film_item.length(); i++) {
            if ((treeWidget_search_result->currentItem() == film_item[i]) ||
                (treeWidget_search_result->currentItem() == desc_item[i]) ||
                (treeWidget_search_result->currentItem() == image_item[i])) {
                FCountMovie = i;
                break;
            }
        }

        if (FCountMovie == (-1)) {
            for (int i = 0; i < film_item.length(); i++) {
                if (treeWidget_search_result->currentItem()->parent() == image_item[i]) {
                    FCountMovie = i;
                    break;
                }
            }
        }

        if (FCountMovie == (-1)) {
            return;
        }

        FimageDownload.clear();
        FimageDownloadDone.clear();
        for (int i=0; i < image_item[FCountMovie]->childCount(); i++) {
            if (image_item[FCountMovie]->child(i)->checkState(0)==Qt::Checked) {
                if (QFile(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_tmp).arg(image_item[FCountMovie]->child(i)->text(0)))).exists()) {
                    FimageDownloadDone << image_item[FCountMovie]->child(i)->text(0);
                } else {
                    plugin_search->download_image(image_item[FCountMovie]->child(i)->text(0),Fdir_tmp);
                    FimageDownload << image_item[FCountMovie]->child(i)->text(0);
                }
            }
        }

        if (FimageDownload.isEmpty()) {
            FparsCommand = 1;
            plugin_search->result_pars_movie(FCountMovie,Fdir_tmp);
        }
    }
}

void MainWindow::on_pushButton_savebuffer_clicked() {
    if ((treeWidget_search_result->topLevelItemCount() > 0) && (film_item.length() > 0) && (treeWidget_search_result->currentItem() != NULL)) {
        label_icon->setText("<img src="":icons/new/information-button.png"" />");

        int count_movie = (-1);

        for (int i = 0; i < film_item.length(); i++) {
            if ((treeWidget_search_result->currentItem() == film_item[i]) ||
                (treeWidget_search_result->currentItem() == desc_item[i]) ||
                (treeWidget_search_result->currentItem() == image_item[i])) {
                count_movie = i;
                break;
            }
        }

        if (count_movie == (-1)) {
            for (int i = 0; i < film_item.length(); i++) {
                if (treeWidget_search_result->currentItem()->parent() == image_item[i]) {
                    count_movie = i;
                    break;
                }
            }
        }

        if (count_movie == (-1)) {
            return;
        }

        FparsCommand = 2;
        plugin_search->result_pars_movie(count_movie,Fdir_tmp);
    }
}

void MainWindow::save_description(bool saveBuf) {
    if (treeWidget_templates->topLevelItemCount() == 0) {
        QMessageBox msgBox;
        msgBox.setText(tr("Not search a template!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    int resave = 0;

    if (saveBuf) {
        for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
            if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {
                QClipboard *clipboard = QApplication::clipboard();
                /*if (i == 0) {
                    clipboard->setText(defaul_templ());
                } else {*/
                    QString text_otb = "";
                    QString FnameFile = "";
                    QString FextFile = "";
                    QString Fcodec = "";
                    QString FpathSave = "";
                    QString FnameFileCover = "";
                    int FsaveTo = (-1);
                    int FsaveCover = (-1);
                    text_otb = pars_template(Fdir_templates+"/"+treeWidget_templates->topLevelItem(i)->text(0),FnameFile,FextFile,Fcodec,FpathSave,FnameFileCover,FsaveTo,FsaveCover);

                    clipboard->setText(text_otb);
                //}
                break;
            }
        }
    } else {
        for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
            if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {                
                    QString text_otb = "";
                    QString FnameFile = "";
                    QString FextFile = "";
                    QString Fcodec = "";
                    QString FpathSave = "";
                    QString FnameFileCover = "";
                    int FsaveTo = (-1);
                    int FsaveCover = (-1);
                    text_otb = pars_template(Fdir_templates+"/"+treeWidget_templates->topLevelItem(i)->text(0),FnameFile,FextFile,Fcodec,FpathSave,FnameFileCover,FsaveTo,FsaveCover);

                    if (checkBox_save_templates->isChecked()) {
                        if (FpathSave.isEmpty()) {
                            QMessageBox msgBox;
                            msgBox.setText(tr("In pattern no tags {;path_save}, save in folder default?"));
                            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                            msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
                            msgBox.setButtonText(QMessageBox::No,tr("No"));
                            msgBox.setDefaultButton(QMessageBox::Yes);
                            msgBox.setIcon(QMessageBox::Question);
                            switch (msgBox.exec()) {
                                case QMessageBox::Yes: {
                                    FpathSave = Fdir_result;
                                    break;
                                }
                                default: {
                                    label_icon->setText("<img src="":icons/new/tick-button.png"" />");
                                    label_icon->setToolTip("");
                                    //num_command = (-1);
                                    //widget_off(true);

                                    return;
                                }
                            }
                        }
                    } else {
                        FpathSave = Fdir_result;
                    }

                    if (!QDir(FpathSave).exists()) {
                        if (!QDir().mkpath(FpathSave)) {
                            QMessageBox msgBox;
                            msgBox.setText(tr("Could not create the folder where you want to save the result, saving cancel."));
                            msgBox.setInformativeText(FpathSave);
                            msgBox.setStandardButtons(QMessageBox::Ok);
                            msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                            msgBox.setDefaultButton(QMessageBox::Ok);
                            msgBox.exec();

                            label_icon->setText("<img src="":icons/new/minus-button.png"" />");
                            label_icon->setToolTip(tr("Could not create the folder where you want to save the result, saving cancel."));
                            //num_command=(-1);
                            //widget_off(true);

                            return;
                        }
                    }

                    if (!FextFile.isEmpty()) {
                        QRegExp regexp("[\\/:*?\"<>|]");
                        FextFile.replace(regexp,"");
                    } else FextFile = "txt";

                    if (!FnameFile.isEmpty()) {
                        QRegExp regexp("[\\/:*?\"<>|]");
                        FnameFile.replace(regexp,"");
                    } else FnameFile = "result";

                    QString FAllSaveName(QDir::toNativeSeparators(FpathSave+"/"+FnameFile+"."+FextFile));
                    QFile file(FAllSaveName);

                    if ((file.exists()) && (checkBox_rewite_file->checkState() == Qt::Unchecked)) {
                        if (resave==0) {
                            QMessageBox msgBox;
                            msgBox.setText(tr("The file already exists."));
                            msgBox.setInformativeText(tr("Rewrite?")+"\n"+FAllSaveName);
                            msgBox.setStandardButtons(QMessageBox::YesToAll | QMessageBox::Yes | QMessageBox::NoToAll | QMessageBox::No);
                            msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
                            msgBox.setButtonText(QMessageBox::No,tr("No"));
                            msgBox.setButtonText(QMessageBox::YesToAll,tr("Yes to all"));
                            msgBox.setButtonText(QMessageBox::NoToAll,tr("No to all"));
                            msgBox.setDefaultButton(QMessageBox::Ok);

                            QString ext = QFileInfo(FAllSaveName).suffix();
                            switch (msgBox.exec()) {
                                case QMessageBox::Yes: break;

                                case QMessageBox::YesToAll:
                                    resave = 1;
                                    break;

                                case QMessageBox::NoToAll:
                                    resave = 2;

                                    FAllSaveName.remove(FAllSaveName.length()-ext.length()-1,ext.length()+1);
                                    for (int i=1;;i++) {
                                        if (!file.exists(FAllSaveName+QString("(%1).").arg(i)+ext)) {
                                            file.setFileName(FAllSaveName+QString("(%1).").arg(i)+ext);
                                            break;
                                        }
                                    }
                                    break;

                                default:
                                    FAllSaveName.remove(FAllSaveName.length()-ext.length()-1,ext.length()+1);
                                    for (int i=1;;i++) {
                                        if (!file.exists(FAllSaveName+QString("(%1).").arg(i)+ext)) {
                                            file.setFileName(FAllSaveName+QString("(%1).").arg(i)+ext);
                                            break;
                                        }
                                    }
                                    break;
                            }
                            file.open(QIODevice::WriteOnly);
                            if (Fcodec.isEmpty())
                                file.write(text_otb.toUtf8());
                            else {
                                QTextCodec *c = QTextCodec::codecForName(Fcodec.toUtf8());
                                file.write(c->fromUnicode(text_otb));
                            }
                            file.close();
                        } else {
                            if (resave==2) {
                                QString ext = QFileInfo(FAllSaveName).suffix();
                                FAllSaveName.remove(FAllSaveName.length()-ext.length()-1,ext.length()+1);
                                for (int i=1;;i++) {
                                    if (!file.exists(FAllSaveName+QString("(%1).").arg(i)+ext)) {
                                        file.setFileName(FAllSaveName+QString("(%1).").arg(i)+ext);
                                        break;
                                    }
                                }
                            }
                            file.open(QIODevice::WriteOnly);
                            if (Fcodec.isEmpty())
                                file.write(text_otb.toUtf8());
                            else {
                                QTextCodec *c = QTextCodec::codecForName(Fcodec.toUtf8());
                                file.write(c->fromUnicode(text_otb));
                            }
                            file.close();
                        }

                    } else {
                        file.open(QIODevice::WriteOnly);
                        if (Fcodec.isEmpty())
                            file.write(text_otb.toUtf8());
                        else {
                            QTextCodec *c = QTextCodec::codecForName(Fcodec.toUtf8());
                            file.write(c->fromUnicode(text_otb));
                        }
                        file.close();
                    }

                    if (FsaveCover == 0) {
                        //save_cover(FnameFileCover);

                        if (FnameFileCover.isEmpty()) FnameFileCover = "result.jpg";
                        QRegExp regexp("[\\/:*?\"<>|]");
                        FnameFileCover.replace(regexp,"");

                        if (!FimageDownloadDone.isEmpty()) {
                            for (int i = 0; i < FimageDownloadDone.count();i++) {
                                FAllSaveName = QDir::toNativeSeparators(FpathSave+"/"+FnameFileCover);
                                QString ext = QFileInfo(FAllSaveName).suffix();
                                if (ext != ".jpg") {
                                    FAllSaveName += ".jpg";
                                    ext = QFileInfo(FAllSaveName).suffix();
                                }
                                file.setFileName(FAllSaveName);

                                if ((file.exists()) && (checkBox_rewite_file->checkState() == Qt::Unchecked)) {
                                    if (resave==0) {
                                        QMessageBox msgBox;
                                        msgBox.setText(tr("The file already exists."));
                                        msgBox.setInformativeText(tr("Rewrite?")+"\n"+FAllSaveName);
                                        msgBox.setStandardButtons(QMessageBox::YesToAll | QMessageBox::Yes | QMessageBox::NoToAll | QMessageBox::No);
                                        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
                                        msgBox.setButtonText(QMessageBox::No,tr("No"));
                                        msgBox.setButtonText(QMessageBox::YesToAll,tr("Yes to all"));
                                        msgBox.setButtonText(QMessageBox::NoToAll,tr("No to all"));
                                        msgBox.setDefaultButton(QMessageBox::Ok);

                                        switch (msgBox.exec()) {
                                            case QMessageBox::Yes: break;

                                            case QMessageBox::YesToAll:
                                                resave = 1;
                                                break;

                                            case QMessageBox::NoToAll:
                                                resave = 2;

                                                FAllSaveName.remove(FAllSaveName.length()-ext.length()-1,ext.length()+1);
                                                for (int i=1;;i++) {
                                                    if (!file.exists(FAllSaveName+QString("(%1).").arg(i)+ext)) {
                                                        file.setFileName(FAllSaveName+QString("(%1).").arg(i)+ext);
                                                        break;
                                                    }
                                                }
                                                break;

                                            default:
                                                FAllSaveName.remove(FAllSaveName.length()-ext.length()-1,ext.length()+1);
                                                for (int j=1;;j++) {
                                                    if (!file.exists(FAllSaveName+QString("(%1).").arg(j)+ext)) {
                                                        file.setFileName(FAllSaveName+QString("(%1).").arg(j)+ext);
                                                        break;
                                                    }
                                                }
                                                break;
                                        }
                                    } else {
                                        if (resave==2) {
                                            FAllSaveName.remove(FAllSaveName.length()-ext.length()-1,ext.length()+1);
                                            for (int j=1;;j++) {
                                                if (!file.exists(FAllSaveName+QString("(%1).").arg(j)+ext)) {
                                                    file.setFileName(FAllSaveName+QString("(%1).").arg(j)+ext);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                file.remove(file.fileName());
                                file.copy(QDir::toNativeSeparators(QString("%1/%2.jpg").arg(Fdir_tmp).arg(FimageDownloadDone[i])),file.fileName());
                            }
                        }
                    }
                //}
            }
        }
    }

    FimageDownload.clear();
    FimageDownloadDone.clear();

    label_icon->setText("<img src="":icons/new/tick-button.png"" />");
    label_icon->setToolTip("");
}

void MainWindow::slot_form_download_image(QString id) {
    plugin_search->download_image(id,Fdir_tmp);
}

void MainWindow::on_pushButton_edit_templates_clicked() {
    save_templates();
    QString n_sh = "";
    for (int i=0;i<treeWidget_templates->topLevelItemCount();i++)
        if (treeWidget_templates->topLevelItem(i)==treeWidget_templates->currentItem()) {
            n_sh = treeWidget_templates->topLevelItem(i)->text(0);
            break;
        }

    Edit_Templates *ab;
    ab = new Edit_Templates(Fdir_templates,n_sh,Ftags_global,Ftags_plug,Ftags_mediafile,this);
    ab->setAttribute(Qt::WA_DeleteOnClose);
    ab->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);
    ab->setWindowModality(Qt::ApplicationModal);
    ab->show();
    connect(ab,SIGNAL(destroyed(QObject*)),this,SLOT(search_templates()));
    return;
}

void MainWindow::search_templates() {
    QSettings settings(QDir::toNativeSeparators(Fdir_settings+"/settings.ini"),QSettings::IniFormat,0);

    QDir dir = Fdir_templates;
    QStringList name_filters;
    name_filters << "*.txt";
    QStringList fileNames = dir.entryList(name_filters,QDir::Files);
    create_item_tree_templ("default",false);
    for (int i = 0; i < fileNames.count(); i++) create_item_tree_templ(fileNames[i],false);
    int size = settings.beginReadArray("template");
    for (int i=0;i<size;i++) {
        settings.setArrayIndex(i);
        QString name = settings.value("name","").toString();
        bool checked = settings.value("checked",false).toBool();
        for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
            if (name == treeWidget_templates->topLevelItem(i)->text(0)) {
                if (checked) treeWidget_templates->topLevelItem(i)->setCheckState(0,Qt::Checked);
            }
        }
    }
    settings.endArray();
}

void MainWindow::save_templates() {
    QSettings settings(QDir::toNativeSeparators(Fdir_settings+"/settings.ini"),QSettings::IniFormat,0);

    settings.remove("template");
    settings.beginWriteArray("template");
    for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
        settings.setArrayIndex(i);
        settings.setValue("name",treeWidget_templates->topLevelItem(i)->text(0));
        if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {
            settings.setValue("checked",true);
        } else {
            settings.setValue("checked",false);
        }
    }
    settings.endArray();

    settings.sync();
}

void MainWindow::on_pushButton_movie_obzor_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a movie"),
                                                     qApp->applicationDirPath(),
                                                     tr("All Files (*.*)"));
    if (!filename.isEmpty()) {
        plugin_movie->result_movie(filename);
        pushButton_movie_obzor->setToolTip(filename);
        pushButton_movie_obzor->setIcon(QIcon(":icons/inbox-film-ok.png"));

        for (int i = 0; i < Ftags_mediafile.count(); i++) qDebug() << QString("%1 - %2").arg(Ftags_mediafile[i]).arg(plugin_movie->result_tags(Ftags_mediafile[i]));
    }
    else {
        if (pushButton_movie_obzor->toolTip().isEmpty()) {
            plugin_movie->init_plug();
            pushButton_movie_obzor->setToolTip("");
            pushButton_movie_obzor->setIcon(QIcon(":icons/inbox-film.png"));
        }
    }
}

void MainWindow::check_stat(QList<int> check) {
    for (int i=0;i<treeWidget_search_result->currentItem()->parent()->childCount();i++) {
        treeWidget_search_result->currentItem()->parent()->child(i)->setCheckState(0,check[i]==0 ? Qt::Checked : Qt::Unchecked);
    }
}

void MainWindow::slot_form_close() {
    bool_fr_pr_image = false;
}

void MainWindow::on_checkBox_save_templates_stateChanged(int state) {
    lineEdit_dir_obzor->setEnabled(state == Qt::Checked ? false : true);
    pushButton_dir_obzor->setEnabled(state == Qt::Checked ? false : true);
}

void MainWindow::on_menu_export_triggered(QAction *act) {
    qDebug() << act->data().toString();
    FopenUrl = act->data().toString();

    if ((treeWidget_search_result->topLevelItemCount() > 0) && (film_item.length() > 0)) {
        label_icon->setText("<img src="":icons/new/information-button.png"" />");

        int count_movie = (-1);

        for (int i = 0; i < film_item.length(); i++) {
            if ((treeWidget_search_result->currentItem() == film_item[i]) ||
                (treeWidget_search_result->currentItem() == desc_item[i]) ||
                (treeWidget_search_result->currentItem() == image_item[i])) {
                count_movie = i;
                break;
            }
        }

        if (count_movie == (-1)) {
            for (int i = 0; i < film_item.length(); i++) {
                if (treeWidget_search_result->currentItem()->parent() == image_item[i]) {
                    count_movie = i;
                    break;
                }
            }
        }

        if (count_movie == (-1)) {
            return;
        }

        FparsCommand = 3;
        plugin_search->result_pars_movie(count_movie,Fdir_tmp);
    }
}

void MainWindow::create_menu_export() {
    disconnect(menu_export,SIGNAL(triggered(QAction*)),this,SLOT(menu_export_triggered(QAction*)));
    QList<QAction *> act = menu_export->actions();
    for (int i = 0; i < act.count(); i++) menu_export->removeAction(act[i]);

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
                    if (rr1.cap(1) == Fcurrent_locale) {
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

QString MainWindow::read_template_description(QString file_name) {
    QFile file_s;
    file_s.setFileName(QDir::toNativeSeparators(file_name));
    qDebug() << file_name;

    if (!file_s.exists()) {
        qDebug() << QString("%1 - %2").arg(tr("not exists file template")).arg(file_name);
        return "";
    }

    if (!file_s.open(QIODevice::ReadOnly)) {
        qDebug() << QString("%1 - %2").arg(tr("not open file template")).arg(file_name);
        return "";
    }

    QString file_shab = QString::fromUtf8(file_s.readAll());
    file_s.close();

    //Search global tegs
    QRegExp reg_exp("\\{;([^=]*)=([^}]*)\\}+(.*)\\n");
    reg_exp.setMinimal(true);
    int pos_r = reg_exp.indexIn(file_shab,0);
    while (pos_r!=(-1)) {
        if (reg_exp.cap(1)=="template_description") {
            if (reg_exp.cap(2).indexOf("\"")!=(-1)) {
                return reg_exp.cap(2).replace("\"","");
            } else return reg_exp.cap(2);
        }

        file_shab.replace(reg_exp.cap(0),"");
        pos_r = reg_exp.indexIn(file_shab,0);
    }
    return "";
}

void MainWindow::on_pushButton_settings_clicked() {
    DialogSettings *dial = new DialogSettings(&Fproxy,&Fcurrent_proxy,&Fproxy_list,&Fclear_tmp_exit,&FSavingModeTraffic);
    dial->setAttribute(Qt::WA_DeleteOnClose);
    dial->exec();
}

void MainWindow::create_items_combobox_plug() {
    comboBox_plugins->clear();

    for (int i = 0; i < FlistPluginsSearch.count(); i++) {
        comboBox_plugins->addItem(FlistPluginsSearch[i].title.isEmpty() ? FlistPluginsSearch[i].filename : FlistPluginsSearch[i].title);

        if (!FlistPluginsSearch[i].icon.isEmpty()) {
            QPixmap pp;
            pp.loadFromData(FlistPluginsSearch[i].icon);
            QIcon icon(pp);
            comboBox_plugins->setItemIcon(i,icon);
        }

        QString desc = "";
        if (FlistPluginsSearch[i].description.isEmpty()) {
            desc = QString(tr("Version: %1")).arg(FlistPluginsSearch[i].version);
        } else {
            desc = QString(tr("Description: %1\nVersion: %2")).arg(FlistPluginsSearch[i].description).arg(FlistPluginsSearch[i].version);
        }

        comboBox_plugins->setItemData(i,desc,Qt::ToolTipRole);

        if (Fcurrent_plugins_search == FlistPluginsSearch[i].filename) comboBox_plugins->setCurrentIndex(i);
    }
}

void MainWindow::comb_plug_indexChanged(int index) {
    if (Fcurrent_plugins_search != FlistPluginsSearch[index].filename) {
        QMessageBox msgBox;
        msgBox.setText(tr("Are you sure you want to change the plugin? Search results will be lost."));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Question);
        switch (msgBox.exec()) {
            case QMessageBox::Yes:
                Fcurrent_plugins_search = FlistPluginsSearch[index].filename;
                load_plugin_search(QDir::toNativeSeparators(Fdir_plugins+"/"+Fcurrent_plugins_search));
                break;
            default:
                for (int i = 0; i < FlistPluginsSearch.count(); i++) {
                    if (FlistPluginsSearch[i].filename == Fcurrent_plugins_search)  {
                        comboBox_plugins->setCurrentIndex(i);
                        return;
                        break;
                    }
                }
                break;
        }
    }
}

void MainWindow::on_pushButton_update_clicked() {

}
