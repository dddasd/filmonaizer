#include "dialogsettings.h"

DialogSettings::DialogSettings(bool *proxy, int *c_proxy, QList<struct_proxy_list> *proxy_list, bool *clear_tmp, bool *saving_mode, QWidget *parent) : QDialog(parent) {
    setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

    Fproxy = proxy;
    FcProxy = c_proxy;
    FlistProxy = proxy_list;
    FclearTemp = clear_tmp;
    FsavingMode = saving_mode;

    for (int i = 0; i < FlistProxy->count(); i++)
        comboBox_proxy->addItem(QString("%1:%2").arg(FlistProxy->value(i).host).arg(FlistProxy->value(i).port));

    checkBox_enable_proxy->setCheckState(*Fproxy ? Qt::Checked : Qt::Unchecked);

    checkBox_clear_temp->setCheckState(*FclearTemp ? Qt::Checked : Qt::Unchecked);

    checkBox_saving_mode_traffic->setCheckState(*FsavingMode ? Qt::Checked : Qt::Unchecked);

    connect(comboBox_proxy,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBox_indexChanged(int)));

    comboBox_proxy->setCurrentIndex(*FcProxy);

    connect(pushButton_close,SIGNAL(clicked(bool)),this,SLOT(close()));

    connect(pushButton_add_proxy,SIGNAL(clicked(bool)),this,SLOT(click_but_add()));

    connect(pushButton_delete_proxy,SIGNAL(clicked(bool)),this,SLOT(click_but_delete()));
}

DialogSettings::~DialogSettings() {
    *Fproxy = (checkBox_enable_proxy->checkState() == Qt::Checked);
    *FcProxy = comboBox_proxy->currentIndex();
    *FclearTemp = (checkBox_clear_temp->checkState() == Qt::Checked);
    *FsavingMode = (checkBox_saving_mode_traffic->checkState() == Qt::Checked);
}

void DialogSettings::click_but_add() {
    if ((lineEdit_ip->text().isEmpty()) || (lineEdit_port->text().isEmpty())) return;

    struct_proxy_list lp;
    lp.host = lineEdit_ip->text();
    lp.port = lineEdit_port->text();
    lp.username = lineEdit_username->text();
    lp.password = lineEdit_password->text();

    FlistProxy->append(lp);

    comboBox_proxy->addItem(QString("%1:%2").arg(lp.host).arg(lp.port));
    comboBox_proxy->setCurrentIndex(comboBox_proxy->count()-1);
}

void DialogSettings::click_but_delete() {
    if (comboBox_proxy->currentIndex() != (-1)) {
        FlistProxy->removeAt(comboBox_proxy->currentIndex());
        comboBox_proxy->removeItem(comboBox_proxy->currentIndex());

        if (comboBox_proxy->count() > 0) {
            comboBox_proxy->setCurrentIndex(0);
        } else {
            lineEdit_ip->setText("");
            lineEdit_port->setText("");
            lineEdit_username->setText("");
            lineEdit_password->setText("");
        }
    }
}

void DialogSettings::comboBox_indexChanged(int index) {
    if (index < 0) {
        lineEdit_ip->setText("");
        lineEdit_port->setText("");
        lineEdit_username->setText("");
        lineEdit_password->setText("");
    } else {
        if (index <= FlistProxy->count()) {
            lineEdit_ip->setText(FlistProxy->value(index).host);
            lineEdit_port->setText(FlistProxy->value(index).port);
            lineEdit_username->setText(FlistProxy->value(index).username);
            lineEdit_password->setText(FlistProxy->value(index).password);
        }
    }
}
