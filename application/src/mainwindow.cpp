#include "mainwindow.h"

MainWindow::MainWindow(bool p, QWidget *parent): QMainWindow(parent) {
    setupUi(this);

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

    read_settings();

    translation(Fcurrent_locale);

    search_plugin();
    if (FfileNamePlugins.isEmpty()) {
        QMessageBox msgBox(QMessageBox::Warning,tr("Fatal error"),tr("Not detected any plugins"),0,this);
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        this->setEnabled(false);
        Fcurrent_plugins = "";
        return;
    }
    bool plg = false;
    for (int i = 0; i < FfileNamePlugins.count();i++) {
        if (FfileNamePlugins[i] == Fcurrent_plugins) {
            plg = true;
            break;
        }
    }
    if (!plg) {
        Fcurrent_plugins = FfileNamePlugins[0];
    }
    plg = load_plugin(QDir::toNativeSeparators(Fdir_plugins+"/"+Fcurrent_plugins));
    if (!plg) {
        for (int i = 0; i < FfileNamePlugins.count(); i++) {
            plg = load_plugin(QDir::toNativeSeparators(Fdir_plugins+"/"+FfileNamePlugins[i]));
            if (plg) {
                Fcurrent_plugins = FfileNamePlugins[i];
                break;
            }
        }
        if (!plg) {
            QMessageBox msgBox(QMessageBox::Warning,tr("Fatal error"),tr("Not detected any plugins"),0,this);
            msgBox.addButton(QMessageBox::Ok);
            msgBox.exec();
            this->setEnabled(false);
            Fcurrent_plugins = "";
            return;
        }
    }

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

    menu_export = new QMenu;
    QAction *act_t;
    //act_t = menu_export->addAction(QIcon(":icons/buzz.png"),QString(tr("Send to Google Buzz")));
    //act_t->setObjectName("Action_bz");
    act_t = menu_export->addAction(QIcon(":icons/twitter.png"),QString(tr("Send to Twitter")));
    act_t->setObjectName("Action_tw");
    //act_t = menu_export->addAction(QIcon(":icons/ff.png"),QString(tr("Send to FriendFeed")));
    //act_t->setObjectName("Action_ff");
    act_t = menu_export->addAction(QIcon(":icons/facebook.png"),QString(tr("Send to Facebook")));
    act_t->setObjectName("Action_fb");
    act_t = menu_export->addAction(QIcon(":icons/vkontakte.png"),QString(tr("Send to VKontakte")));
    act_t->setObjectName("Action_vk");
    pushButton_export->setMenu(menu_export);
    //connect(menu_export,SIGNAL(triggered(QAction*)),this,SLOT(on_menu_export_triggered(QAction*)));


    movie = new QMovie(":/icons/loading.gif");
    connect(movie,SIGNAL(frameChanged(int)),this,SLOT(setItemIcon(int)));
    if (movie->loopCount() != -1) connect(movie,SIGNAL(finished()),movie,SLOT(start()));

    expand_s = true;

    lineEdit_dir_obzor->setText(Fdir_result);

    if (FtemplateCheck) checkBox_save_templates->setCheckState(Qt::Checked);

    if (FrewriteFile) radioButton_rewrite_yes->setChecked(true);
    else radioButton_rewrite_question->setChecked(true);

    FparsCommand = (-1);
}

MainWindow::~MainWindow() {    
    if (checkBox_save_templates->checkState()==Qt::Checked) FtemplateCheck = true;
    else FtemplateCheck = false;

    if (radioButton_rewrite_yes->isChecked()) FrewriteFile = true;
    else FrewriteFile = false;

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
            if ((Fcurrent_proxy < 0) && (Fcurrent_proxy >= Fproxy_list.count())) {
                Fcurrent_proxy = 0;
            }
            plugin_f->set_proxy(Fproxy,Fproxy_list[Fcurrent_proxy].host,Fproxy_list[Fcurrent_proxy].port,
                                Fproxy_list[Fcurrent_proxy].username,Fproxy_list[Fcurrent_proxy].password);
        } else {
            plugin_f->set_proxy(false,"","","","");
        }
        plugin_f->result_search_movie(searchLineEdit_movie->text());
    }
}

bool MainWindow::load_plugin(QString fileName) {
    if (!QFile(fileName).exists()) return false;
    if (pluginLoader.isLoaded()) pluginLoader.unload();
    pluginLoader.setFileName(fileName);
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        plugin_f = qobject_cast<QInterfacePluginSearch *>(plugin);
        if (!plugin_f) return false;
        else {
            plugin_f->init_plug();
            QObject *ttt = plugin_f->notifyer();
            connect(ttt,SIGNAL(signalSearch(QList<QString>,int,QString)),this,SLOT(slotSearch(QList<QString>,int,QString)));
            connect(ttt,SIGNAL(signalPars(int,QString)),this,SLOT(slotPars(int,QString)));
            connect(ttt,SIGNAL(signalSmallImage(int,QList<QString>)),this,SLOT(slotSmallImage(int,QList<QString>)));
            Ftags_plug.clear();
            Ftags_plug = plugin_f->listTags();
            qDebug() << Ftags_plug;
            return true;
        }
    } else {
        qDebug() << pluginLoader.errorString();
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
                int FsaveTo = (-1);
                if (j==0) text_otb = defaul_templ();
                else text_otb = pars_template(Fdir_templates+"/"+templ[j],FnameFile,FextFile,Fcodec,FpathSave,FsaveTo);

                Previewdesc *prev = new Previewdesc(text_otb,"",templ,j);
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

    image_item[FsmallImageClick]->setExpanded(true);
    label_icon->setText("<img src="":icons/new/tick-button.png"" />");
    label_icon->setToolTip("");

    FsmallImageClick = (-1);
}

QString MainWindow::template_change(QString templ) {
    QString text_otb = "";
    QString FnameFile = "";
    QString FextFile = "";
    QString Fcodec = "";
    QString FpathSave = "";
    int FsaveTo = (-1);
    if (templ=="default") text_otb = defaul_templ();
    else text_otb = pars_template(Fdir_templates+"/"+templ,FnameFile,FextFile,Fcodec,FpathSave,FsaveTo);

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
            int j = (-1);
            for (int k=0;k<film_item.length();k++)
                if (ret->parent()==film_item[k]) {
                    j=k;
                    break;
                }
            if (j == (-1)) return;
            FparsCommand = 0;
            icon_ch = ret;
            movie->start();
            plugin_f->result_pars_movie(j,Fdir_tmp);
        }
    for (int i=0;i<image_item.length();i++)
        if (ret==image_item[i]) {
            int j = (-1);
            for (int k=0;k<film_item.length();k++)
                if (ret->parent()==film_item[k]) {
                    j=k;
                    break;
                }
            if (j == (-1)) return;
            FsmallImageClick = j;
            icon_ch = ret;
            movie->start();
            plugin_f->result_search_small_image(j,Fdir_tmp);
        }
}

void MainWindow::on_pushButton_expand_clicked() {
    expanded(expand_s);
}

void MainWindow::expanded(bool ex) {
    if(ex) {
        expand = this->height() - treeWidget_templates->height() - 4;
        collapse = this->height();

        groupBox_rewrite->setVisible(false);
        groupBox_save->setVisible(false);
        treeWidget_templates->setVisible(false);
        pushButton_edit_templates->setVisible(false);
        pushButton_about->setVisible(false);
        pushButton_set_proxy->setVisible(false);
        pushButton_packet_file->setVisible(false);


        this->setMinimumSize(this->width(),expand);
        this->setMaximumSize(this->width(),expand);
        this->resize(this->width(),expand);

        button_expand->setIcon(QIcon(":icons/new/navigation-270-button.png"));
        button_expand->setToolTip(tr("Expand"));
        expand_s = false;
    }
    else {
        groupBox_rewrite->setVisible(true);
        groupBox_save->setVisible(true);
        treeWidget_templates->setVisible(true);
        pushButton_edit_templates->setVisible(true);
        pushButton_about->setVisible(true);
        pushButton_set_proxy->setVisible(true);
        pushButton_packet_file->setVisible(true);

        this->setMinimumSize(this->width(),collapse);
        this->setMaximumSize(this->width(),collapse);
        this->resize(this->width(),collapse);

        button_expand->setIcon(QIcon(":icons/new/navigation-090-button.png"));
        button_expand->setToolTip(tr("Collapse"));
        expand_s = true;
    }
}

QString MainWindow::defaul_templ() {
    QString ret = "";
    for (int i = 0; i < Ftags_plug.count(); i++) {
        ret.append(QString("%1 - %2\n").arg(Ftags_plug[i]).arg(plugin_f->result_tags(Ftags_plug[i])));
    }
    return ret;
}

void MainWindow::on_pushButton_about_clicked() {
    AboutDialog *ab;
    if ((Fproxy) && (Fproxy_list.count() > 0)) {
        if ((Fcurrent_proxy < 0) && (Fcurrent_proxy >= Fproxy_list.count())) {
            Fcurrent_proxy = 0;
        }
        ab = new AboutDialog(Fcurrent_locale,Fproxy,Fproxy_list[Fcurrent_proxy].host,Fproxy_list[Fcurrent_proxy].port,
                             Fproxy_list[Fcurrent_proxy].username,Fproxy_list[Fcurrent_proxy].password);
    } else {
        ab = new AboutDialog(Fcurrent_locale);
    }
    ab->setAttribute(Qt::WA_DeleteOnClose);
    ab->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    connect(ab,SIGNAL(ch_locale(QString)),this,SLOT(translation(QString)));
    //connect(ab,SIGNAL(save_version(QString,int)),this,SLOT(save_version(QString,int)));
    ab->show();
}

void MainWindow::on_pushButton_set_proxy_clicked() {
    Set_proxy *ab = new Set_proxy(Fproxy,Fcurrent_proxy,Fproxy_list,this);
    ab->setAttribute(Qt::WA_DeleteOnClose);
    ab->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    connect(ab,SIGNAL(save_settings(bool,int,QList<struct_proxy_list>)),this,SLOT(save_settings_proxy(bool,int,QList<struct_proxy_list>)));
    ab->show();
}

void MainWindow::save_settings_proxy(bool proxy, int c_proxy, QList<struct_proxy_list> proxy_list) {
    Fproxy_list = proxy_list;
    Fproxy = proxy;
    Fcurrent_proxy = c_proxy;
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
    create_item_tree_templ("default",false);
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
    Fcurrent_plugins = settings.value("current_plugins","").toString();
    Fcurrent_proxy = settings.value("current_proxy",(-1)).toInt();
    Fproxy = settings.value("enable_proxy",false).toBool();
    FtemplateCheck = settings.value("get_template",false).toBool();
    FrewriteFile = settings.value("rewrite_a_file",false).toBool();
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
    settings.setValue("current_plugins",Fcurrent_plugins);
    settings.setValue("current_proxy",Fcurrent_proxy);
    settings.setValue("enable_proxy",Fproxy);
    settings.setValue("get_template",FtemplateCheck);
    settings.setValue("rewrite_a_file",FrewriteFile);
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

    treeWidget_templates->insertTopLevelItem(0,new_item);
}

void MainWindow::translation(QString i) {
    qDebug() << i;
    if (i == "ru_RU") {
        if (trans.load(Fdir_locale+QDir::separator()+"filmonaizer_ru_RU")) {
            qApp->installTranslator(&trans);
            retranslateUi(this);
            Fcurrent_locale = i;
            return;
        }
    }
    qApp->removeTranslator(&trans);
    retranslateUi(this);
    Fcurrent_locale = "en_US";
}

void MainWindow::search_plugin() {
    QDir dir = Fdir_plugins;
    QStringList name_filters;
#ifdef Q_OS_WIN
    name_filters << "*.dll";
#else
    name_filters << "*.so";
#endif
    FfileNamePlugins = dir.entryList(name_filters,QDir::Files);
}

QString MainWindow::pars_template(QString file_name,
                                  QString &FSetNameFile,
                                  QString &FSetExtFile,
                                  QString &FSetCodec,
                                  QString &FSetPathSave,
                                  int &FSetSaveTo) {
    /*
    Global Settings:
    {;name_file}		имя файла
    {;ext_file}			расширение файла
    {;cut_end}			окончание строки если превышает заданный размер
    {;codec}			кодировка в которой сохранять
    {;path_save}		путь до директории, в которую сохраняем файлы
    {;save_to}			вариант сохранения: 0 - в буфер, 1 - в файл, 2 - в оба места

    {;custom}			пользовательские переменные (не реализовано)
    {;use_short_url}	использовать сервер коротких ссылок (отключено) (не реализовано)
    {;save_cover}		сохранять обложки или нет: 0 - сохранять, 1 - не сохранять (не реализовано)
    {;name_file_cover}	имя файла обложки (не реализовано)*/

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
    int FsaveTo = (-1);

    //Search global tegs
    QRegExp reg_exp("\\{;([^=]*)=([^}]*)\\}+(.*)\\n");
    reg_exp.setMinimal(true);
    int pos_r = reg_exp.indexIn(file_shab,0);
    while (pos_r!=(-1)) {
        if (reg_exp.cap(1)=="name_file") {
            if (reg_exp.cap(2).indexOf("\"")!=(-1)) {
                FnameFile = reg_exp.cap(2).replace("\"","");
            } else {
                FnameFile = plugin_f->result_tags(QString("{:%1}").arg(reg_exp.cap(2)));
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

        file_shab.replace(reg_exp.cap(0),"");
        pos_r = reg_exp.indexIn(file_shab,0);
    }
    //\Search global tegs

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

            QString str_r = plugin_f->result_tags(temp_str);
            if (!razd.isEmpty()) str_r.replace(",",razd);
            if (length_m!=(-1)) {
                if (str_r.length()>length_m) {
                    str_r = str_r.mid(0,length_m)+FcutEnd;
                }
            }
            file_shab.replace(reg_exp.cap(0),str_r);
        } else {
            file_shab.replace(reg_exp.cap(0),plugin_f->result_tags(reg_exp.cap(0)));
        }
        pos_r = reg_exp.indexIn(file_shab,0);
    }

    FSetNameFile = FnameFile;
    FSetExtFile = FextFile;
    FSetCodec = Fcodec;
    FSetPathSave = FpathSave;
    FSetSaveTo = FsaveTo;
    return file_shab;
}


void MainWindow::on_pushButton_savefile_clicked() {
    if ((treeWidget_search_result->topLevelItemCount() > 0) && (film_item.length() > 0)) {
        label_icon->setText("<img src="":icons/new/information-button.png"" />");

        int i = (-1);
        for (int j = 0; j < film_item.length(); j++) {

            if ((treeWidget_search_result->currentItem() == film_item[j]) ||
                (treeWidget_search_result->currentItem()->parent() == film_item[j])/* ||
                (treeWidget_search_result->currentItem()->parent()->parent() == film_item[j])*/) {
                i = j;
                break;
            }
        }

        if (i==(-1)) {
            return;
        }

        FparsCommand = 1;
        plugin_f->result_pars_movie(i,Fdir_tmp);
    }
}

void MainWindow::on_pushButton_savebuffer_clicked() {
    if ((treeWidget_search_result->topLevelItemCount() > 0) && (film_item.length() > 0)) {
        label_icon->setText("<img src="":icons/new/information-button.png"" />");

        int i = (-1);
        for (int j = 0; j < film_item.length(); j++) {
            if ((treeWidget_search_result->currentItem() == film_item[j]) ||
                (treeWidget_search_result->currentItem()->parent() == film_item[j])/* ||
                (treeWidget_search_result->currentItem()->parent()->parent() == film_item[j])*/) {
                i = j;
                break;
            }
        }

        if (i==(-1)) {
            return;
        }

        FparsCommand = 2;
        plugin_f->result_pars_movie(i,Fdir_tmp);
    }
}

void MainWindow::save_description(bool saveBuf) {
    int resave = 0;

    if (saveBuf) {
        for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
            if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {
                QClipboard *clipboard = QApplication::clipboard();
                if (i == 0) {
                    clipboard->setText(defaul_templ());
                } else {
                    QString text_otb = "";
                    QString FnameFile = "";
                    QString FextFile = "";
                    QString Fcodec = "";
                    QString FpathSave = "";
                    int FsaveTo = (-1);
                    text_otb = pars_template(Fdir_templates+"/"+treeWidget_templates->topLevelItem(i)->text(0),FnameFile,FextFile,Fcodec,FpathSave,FsaveTo);

                    clipboard->setText(text_otb);
                }
                break;
            }
        }
    } else {
        for (int i=0;i<treeWidget_templates->topLevelItemCount();i++) {
            if (treeWidget_templates->topLevelItem(i)->checkState(0)==Qt::Checked) {
                if (i == 0) { //default
                    QFile file(QDir::toNativeSeparators(Fdir_result+"/result.txt"));

                    if ((file.exists()) && (radioButton_rewrite_question->isChecked())) {
                        QMessageBox msgBox;
                        msgBox.setText(tr("The file already exists."));
                        msgBox.setInformativeText(tr("Rewrite?")+"\n"+QString(Fdir_result+"/result.txt"));
                        msgBox.setStandardButtons(QMessageBox::YesToAll | QMessageBox::Yes | QMessageBox::NoToAll | QMessageBox::No);
                        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
                        msgBox.setButtonText(QMessageBox::No,tr("No"));
                        msgBox.setButtonText(QMessageBox::YesToAll,tr("Yes to all"));
                        msgBox.setButtonText(QMessageBox::NoToAll,tr("No to all"));
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        switch (msgBox.exec()) {
                            case QMessageBox::No:
                                for (int i=1;;i++){
                                    if (!file.exists(QDir::toNativeSeparators(QString(Fdir_result+"/result(%1).txt")).arg(i))) {
                                        file.setFileName(QDir::toNativeSeparators(QString(Fdir_result+"/result(%1).txt")).arg(i));
                                        break;
                                    }
                                }
                                break;
                            case QMessageBox::YesToAll:
                                resave = 1;
                                break;
                            case QMessageBox::NoToAll:
                                resave = 2;
                                for (int i=1;;i++){
                                    if (!file.exists(QDir::toNativeSeparators(QString(Fdir_result+"/result(%1).txt")).arg(i))) {
                                        file.setFileName(QDir::toNativeSeparators(QString(Fdir_result+"/result(%1).txt")).arg(i));
                                        break;
                                    }
                                }
                                break;
                        }
                    }
                    file.open(QIODevice::WriteOnly);
                    file.write(defaul_templ().toUtf8());
                    file.close();
                } else {
                    QString text_otb = "";
                    QString FnameFile = "";
                    QString FextFile = "";
                    QString Fcodec = "";
                    QString FpathSave = "";
                    int FsaveTo = (-1);
                    text_otb = pars_template(Fdir_templates+"/"+treeWidget_templates->topLevelItem(i)->text(0),FnameFile,FextFile,Fcodec,FpathSave,FsaveTo);

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

                    if ((file.exists()) && (radioButton_rewrite_question->isChecked())) {
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
                }
            }
        }
    }

    label_icon->setText("<img src="":icons/new/tick-button.png"" />");
    label_icon->setToolTip("");
}
