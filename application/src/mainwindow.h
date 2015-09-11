#include "ui_mainwindow.h"

#include <QList>
#include <QString>
#include <QPluginLoader>
#include <QDir>
#include <QLabel>
#include <QPushButton>
#include <QAction>
#include <QMenu>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QMovie>
#include <QTreeWidgetItem>
#include <QSettings>
#include <QTranslator>
#include <QMessageBox>
#include <QFileInfo>

#include "qinterfacepluginsearch.h"
#include "qinterfacepluginmovie.h"

#include "previewdesc.h"
#include "aboutdialog.h"
#include "set_proxy.h"
#include "previewcover.h"
#include "edit_templates.h"

class MainWindow : public QMainWindow, private Ui::MainWindow_filmonaizer {
    Q_OBJECT
public:
    MainWindow(bool p, QWidget *parent = 0);
    ~MainWindow();

private:
    void search_plugin();

    bool load_plugin_search(QString);
    bool load_plugin_movie(QString);

    QList<QTreeWidgetItem*> film_item;
    QList<QTreeWidgetItem*> desc_item;
    QList<QTreeWidgetItem*> image_item;
    QList<QTreeWidgetItem*> sm_image_item;
    QLabel *label_zap, *label_icon;
    QPushButton *button_expand;
    QMenu *menu_export;

    QList<QString> Ffilename_plugin_search;
    QList<QString> Ffilename_plugin_movie;

    QString Fcurrent_plugins_search;
    QString Fcurrent_plugins_movie;

    QPluginLoader pluginLoaderSearch;
    QPluginLoader pluginLoaderMovie;

    QInterfacePluginSearch *plugin_search;
    QInterfacePluginMovie *plugin_movie;

    bool Fplug_movie_load;

    QList<QString> Ftags_global; //теги глобальные
    QList<QString> Ftags_plug; //теги плагина поиска
    QList<QString> Ftags_mediafile; //теги плагина МедиаФайла

    QMovie *movie;
    QTreeWidgetItem *icon_ch;

    QString Fdir_tmp, Fdir_settings, Fdir_templates, Fdir_locale, Fdir_result, Fdir_plugins;

    bool expand_s;
    int expand, collapse;

    //PROXY
    bool Fproxy;
    int Fcurrent_proxy;
    QList<struct_proxy_list> Fproxy_list;
    ////

    //
    QList<QString> Fhistory_search;

    //
    QString Fcurrent_locale;
    QTranslator trans;

    //
    bool Fclear_tmp_exit;


    //
    bool FtemplateCheck;

    //
    bool FrewriteFile;

    int FparsCommand; // 0 - превью окно, 1 - сохранить в файл, 2 - сохранить в буфер

    int FsmallImageClick; //id - куда кликнули

    PreviewCover *form_pr_image;

    void expanded(bool);
    QString defaul_templ();

    void read_settings();
    void write_settings();

    void search_templates();
    void save_templates();

    void create_item_tree_templ(QString,bool);

    QString pars_template(QString,QString&,QString&,QString&,QString&,int&);

    void save_description(bool);
private slots:
    void slotSearch(QList<QString>,int,QString);
    void slotPars(int,QString);
    void slotSmallImage(int,QList<QString>);
    void slotDownloadImage(int,int);

    void setItemIcon(int);

    void itemExpanded(QTreeWidgetItem*);
    void itemCollapsed(QTreeWidgetItem*);
    void itemDoubleClicked(QTreeWidgetItem*,int);

    void save_settings_proxy(bool,int,QList<struct_proxy_list>);

    void translation(QString);

    QString template_change(QString);

    void slot_form_download_image(int id);

    void on_pushButton_search_clicked();
    void on_pushButton_savefile_clicked();
    void on_pushButton_savebuffer_clicked();
    void on_pushButton_expand_clicked();
    void on_pushButton_about_clicked();
    void on_pushButton_set_proxy_clicked();
    void on_pushButton_edit_templates_clicked();
    void on_pushButton_movie_obzor_clicked();
};
