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
#include "previewcover.h"
#include "edit_templates.h"
#include "dialog_variables.h"
#include "dialogsettings.h"

struct struct_plugins_list {
    QString title;
    QString description;
    QString version;
    QString filename;
    QByteArray icon;
};

class MainWindow : public QMainWindow, private Ui::MainWindow_filmonaizer {
    Q_OBJECT
public:
    MainWindow(bool p, QWidget *parent = 0);
    ~MainWindow();

private:

/*Forms*/
    QList<QTreeWidgetItem*> film_item;
    QList<QTreeWidgetItem*> desc_item;
    QList<QTreeWidgetItem*> image_item;
    QList<QTreeWidgetItem*> sm_image_item;
    QLabel *label_zap, *label_icon;
    QPushButton *button_expand;
    QMenu *menu_export;

    QMovie *movie;
    QTreeWidgetItem *icon_ch;

    bool expand_s;
    int expand, collapse;

    QTranslator trans;

    PreviewCover *form_pr_image;
    bool bool_fr_pr_image;
/*\Forms*/

/*Plugins*/
    QList<struct_plugins_list> FlistPluginsSearch;
    //QList<QString> Ffilename_plugin_search;
    QList<struct_plugins_list> FlistPluginsMovie;
    //QList<QString> Ffilename_plugin_movie;

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

/*\Plugins*/

/*Settings*/
    QString Fdir_tmp, Fdir_settings, Fdir_templates, Fdir_locale, Fdir_result, Fdir_plugins;
    //PROXY
    bool Fproxy;
    int Fcurrent_proxy;
    QList<struct_proxy_list> Fproxy_list;
    QList<QString> Fhistory_search;
    QString Fcurrent_locale;
    bool Fclear_tmp_exit;  
    bool FtemplateCheck;
    bool FrewriteFile;
    bool FSavingModeTraffic;
/*\Settings*/

    QList<QString> FimageDownload; //список картинок которые нужно скачать
    QList<QString> FimageDownloadDone; //Список скачанных картинок

    int FparsCommand; // 0 - превью окно, 1 - сохранить в файл, 2 - сохранить в буфер, 3 - открыть в ссылке
    QString FopenUrl;

    int FsmallImageClick; //id - куда кликнули

    int FCountMovie;

    void search_plugin();

    bool load_plugin_search(QString);
    bool load_plugin_movie(QString);

    void expanded(bool);

    void read_settings();
    void write_settings();

    void create_menu_export();

    void search_templates();
    void save_templates();

    void create_item_tree_templ(QString,bool);

    QString pars_template(QString,QString&,QString&,QString&,QString&,QString&,int&,int&);

    void save_description(bool);

    QString read_template_description(QString);

    void read_list_plugin(QString filename, struct_plugins_list *pl);

    void create_items_combobox_plug();
private slots:
    void slotSearch(QList<QString>,int,QString);
    void slotPars(int,QString);
    void slotSmallImage(int,QList<QString>);
    void slotDownloadImage(QString,int);

    void setItemIcon(int);

    void itemExpanded(QTreeWidgetItem*);
    void itemCollapsed(QTreeWidgetItem*);
    void itemDoubleClicked(QTreeWidgetItem*,int);

    void translation(QString);

    QString template_change(QString);

    void check_stat(QList<int> check);

    void slot_form_download_image(QString id);
    void slot_form_close();

    void on_pushButton_search_clicked();
    void on_pushButton_savefile_clicked();
    void on_pushButton_savebuffer_clicked();
    void on_pushButton_expand_clicked();
    void on_pushButton_about_clicked();
    void on_pushButton_edit_templates_clicked();
    void on_pushButton_movie_obzor_clicked();
    void on_pushButton_settings_clicked();
    void on_pushButton_update_clicked();
    void on_checkBox_save_templates_stateChanged(int state);
    void on_menu_export_triggered(QAction*);

    void comb_plug_indexChanged(int);
};
