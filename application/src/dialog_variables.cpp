#include "dialog_variables.h"

dialog_variables::dialog_variables(QList<list_variables> *vr, QWidget *parent /*= 0*/ ) {
	Q_UNUSED(parent);
    vrb = vr;
	this->setWindowTitle(tr("variables"));
	this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);

	QVBoxLayout *boxx = new QVBoxLayout(this);
	QLabel *l = new QLabel(tr("the template is user-defined variables"),this);
	QFont f(l->font());
	f.setBold(true);
	f.setItalic(true);
	l->setFont(f);
	boxx->addWidget(l);
	QGridLayout *boxx1 = new QGridLayout(this);
    for (int i=0;i<vrb->count();i++)
	{
        QLabel *lab = new QLabel(vrb->value(i).name_v,this);
		lab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
		QComboBox *comb = new QComboBox(this);
		comb->setObjectName(QString("param%1").arg(i+1));
        for (int j=0;j<vrb->value(i).value_v.count();j++) {
            comb->addItem(vrb->value(i).value_v[j]);
		}
		boxx1->addWidget(lab,i,0);
		boxx1->addWidget(comb,i,1);
	}
	boxx1->setColumnStretch(0,0);
	boxx1->setColumnStretch(1,10);
	boxx->addLayout(boxx1);
	QPushButton *but = new QPushButton(this);
	but->setText(tr("Select"));
	connect(but,SIGNAL(clicked()),this,SLOT(clicked_but()));
	boxx->addWidget(but);
	this->setLayout(boxx);
}

dialog_variables::~dialog_variables() {
	QList<QComboBox *> list_c = this->findChildren<QComboBox *>(QRegExp("param*"));
	for(int i=0;i<list_c.count();i++) {
        list_variables temp_l = vrb->value(i);
        temp_l.value_s = list_c[i]->currentText();
        vrb->replace(i,temp_l);
    }
}

void dialog_variables::clicked_but() {
	close();
}
