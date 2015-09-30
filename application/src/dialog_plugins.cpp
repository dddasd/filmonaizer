#include "dialog_plugins.h"

DialogPlugins::DialogPlugins(QList<QString> plug_search, QString cureent_pl_s, QList<QString> plug_media, QString cureent_pl_m, QWidget *parent) : QWidget(parent) {
    gr_search = new QGroupBox(tr("Plugins Search"),this);
    gr_media = new QGroupBox(tr("Plugins Media"),this);

    QVBoxLayout *v1 = new QVBoxLayout(gr_search);
    for (int i = 0; i < plug_search.count(); i++) {
        QRadioButton *check = new QRadioButton(plug_search[i],gr_search);
        if (cureent_pl_s == plug_search[i]) check->setChecked(true);
        v1->addWidget(check);
    }
    gr_search->setLayout(v1);
    if (plug_search.count() < 2) gr_search->setEnabled(false);

    QVBoxLayout *v2 = new QVBoxLayout(gr_media);
    for (int i = 0; i < plug_media.count(); i++) {
        QRadioButton *check = new QRadioButton(plug_media[i],gr_media);
        if (cureent_pl_m == plug_media[i]) check->setChecked(true);
        v2->addWidget(check);
    }
    gr_media->setLayout(v2);
    if (plug_media.count() < 2) gr_media->setEnabled(false);

    QPushButton *b1 = new QPushButton(tr("Confirm changes"),this);
    connect(b1,SIGNAL(clicked(bool)),this,SLOT(click_save()));
    QPushButton *b2 = new QPushButton(tr("Close Window"),this);
    connect(b2,SIGNAL(clicked(bool)),this,SLOT(close()));

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->addWidget(b1);
    h1->addWidget(b2);

    QVBoxLayout *main_g = new QVBoxLayout(this);
    main_g->addWidget(gr_search);
    main_g->addWidget(gr_media);
    main_g->addLayout(h1);

    this->setLayout(main_g);
}

void DialogPlugins::click_save() {
    QString pl_s, pl_m;
    QList<QRadioButton *> allButtons = gr_search->findChildren<QRadioButton *>();
    for (int i = 0; i < allButtons.count(); i++) {
        if (allButtons[i]->isChecked()) {
            pl_s = allButtons[i]->text();
            break;
        }
    }

    allButtons = gr_media->findChildren<QRadioButton *>();
    for (int i = 0; i < allButtons.count(); i++) {
        if (allButtons[i]->isChecked()) {
            pl_m = allButtons[i]->text();
            break;
        }
    }

    emit save_plug(pl_s,pl_m);

    close();
}
