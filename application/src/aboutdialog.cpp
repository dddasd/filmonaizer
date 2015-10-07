#include "aboutdialog.h"

AboutDialog::AboutDialog(QString cur_l, QWidget *parent)  : QDialog(parent)
{
    setupUi(this);
    setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    setModal(true);

    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

    label_main->setText(label_main->text().arg(qApp->applicationVersion()).arg(QT_VERSION_STR));

    if (cur_l=="ru_RU")
        comboBox->setCurrentIndex(1);
    else
        comboBox->setCurrentIndex(0);
}

void AboutDialog::on_comboBox_activated(int index)
{
    if (index!=(-1)) {
        switch (index) {
            case 0: {
                emit ch_locale("en_US");
                break;
            }
            case 1: {
                emit ch_locale("ru_RU");
                break;
            }
        }
        retranslateUi(this);
		label_main->setText(label_main->text().arg(qApp->applicationVersion()).arg(QT_VERSION_STR));
        comboBox->setCurrentIndex(index);
    }
}
