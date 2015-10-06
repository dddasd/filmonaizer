#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include "ui_settings.h"

struct struct_proxy_list {
    QString host;
    QString port;
    QString username;
    QString password;
};

class DialogSettings : public QDialog, public Ui::Dialog_settings
{
    Q_OBJECT
public:
    DialogSettings(bool *proxy, int *c_proxy, QList<struct_proxy_list> *proxy_list, bool *clear_tmp, bool *saving_mode, QWidget *parent = 0);
    ~DialogSettings();

private:
    bool *Fproxy;
    int *FcProxy;
    QList<struct_proxy_list> *FlistProxy;
    bool *FclearTemp;
    bool *FsavingMode;

private slots:
    void click_but_add();
    void click_but_delete();
    void comboBox_indexChanged(int);
};

#endif // DIALOGSETTINGS_H
