#ifndef SET_PROXY_H
#define SET_PROXY_H

#include "ui_set_proxy.h"
//#include "global_struct.h"
#include <QDialog>

struct struct_proxy_list {
    QString host;
    QString port;
    QString username;
    QString password;
};


class Set_proxy : public QDialog, public Ui::Form_setproxy
{
    Q_OBJECT
public:
    Set_proxy(bool proxy, int c_proxy, QList<struct_proxy_list> proxy_list, QWidget *parent = 0);

private:
    QList<struct_proxy_list> l_p;
    int current_proxy;

signals:
    void save_settings(bool,int,QList<struct_proxy_list>);

private slots:
    void on_button_save_proxy_clicked();
    void on_button_del_proxy_clicked();
    void on_button_add_proxy_clicked();
    void on_button_choose_clicked();
    void on_list_proxy_currentIndexChanged(int);
    //void validating(QString ret);

};

#endif // SET_PROXY_H
