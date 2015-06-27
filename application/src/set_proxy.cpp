#include "set_proxy.h"

Set_proxy::Set_proxy(bool proxy, int c_proxy, QList<struct_proxy_list> proxy_list, QWidget *parent) : QDialog(parent) {
    setupUi(this);
    setModal(true);

    l_p = proxy_list;

    checkBox_proxy->setChecked(proxy);
    groupBox->setEnabled(proxy);

    for (int i=0;i<l_p.length();i++) {
        list_proxy->addItem(l_p[i].host+":"+l_p[i].port);
    }
    current_proxy = c_proxy;

    if (l_p.count() == 0) {
        text_ip_proxy->setText("");
        text_port_proxy->setText("");
        text_user_proxy->setText("");
        text_pass_proxy->setText("");
        list_proxy->setCurrentIndex((-1));
        current_proxy = (-1);
    } else {
        if ((current_proxy >= 0) && (current_proxy < l_p.count())) {
            text_ip_proxy->setText(l_p[current_proxy].host);
            text_port_proxy->setText(l_p[current_proxy].port);
            text_user_proxy->setText(l_p[current_proxy].username);
            text_pass_proxy->setText(l_p[current_proxy].password);
            list_proxy->setCurrentIndex(current_proxy);
        } else {
            text_ip_proxy->setText("");
            text_port_proxy->setText("");
            text_user_proxy->setText("");
            text_pass_proxy->setText("");
            list_proxy->setCurrentIndex((-1));
            current_proxy = (-1);
        }
    }

    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

void Set_proxy::on_list_proxy_currentIndexChanged(int index) {
    current_proxy = index;
    if (index==(-1)) {
        text_ip_proxy->setText("");
        text_port_proxy->setText("");
        text_user_proxy->setText("");
        text_pass_proxy->setText("");
    }
    else {
        text_ip_proxy->setText(l_p[index].host);
        text_port_proxy->setText(l_p[index].port);
        text_user_proxy->setText(l_p[index].username);
        text_pass_proxy->setText(l_p[index].password);
    }
}

void Set_proxy::on_button_choose_clicked() {
    save_settings(checkBox_proxy->isChecked(),current_proxy,l_p);

    close();
}

void Set_proxy::on_button_add_proxy_clicked() {
    struct_proxy_list pp;
    pp.host = text_ip_proxy->text();
    pp.port = text_port_proxy->text();
    pp.username = text_user_proxy->text();
    pp.password = text_pass_proxy->text();
    l_p.append(pp);
    list_proxy->addItem(text_ip_proxy->text()+":"+text_port_proxy->text());
    list_proxy->setCurrentIndex(l_p.length()-1);
    current_proxy = l_p.length()-1;
}

void Set_proxy::on_button_del_proxy_clicked() {
    if (current_proxy!=(-1)) {
        l_p.removeAt(current_proxy);
        list_proxy->removeItem(current_proxy);

        if (list_proxy->count() >= 0) {
            current_proxy = 0;
            list_proxy->setCurrentIndex(0);
        } else {
            current_proxy = (-1);
            list_proxy->setCurrentIndex((-1));
        }
    }
}

void Set_proxy::on_button_save_proxy_clicked() {
    struct_proxy_list pp;
    pp.host = text_ip_proxy->text();
    pp.port = text_port_proxy->text();
    pp.username = text_user_proxy->text();
    pp.password = text_pass_proxy->text();
    l_p.append(pp);
    list_proxy->addItem(text_ip_proxy->text()+":"+text_port_proxy->text());
    list_proxy->setCurrentIndex(l_p.length()-1);
    current_proxy = l_p.length()-1;
}
