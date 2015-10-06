#ifndef DIALOG_VARIABLES_H
#define DIALOG_VARIABLES_H

#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QList>

struct list_variables {
    QString tag;
    QString name_v;
    QList<QString> value_v;
    QString value_s;
};

class dialog_variables : public QDialog
{
	Q_OBJECT
public:
    dialog_variables(QList<list_variables> *vr, QWidget *parent = 0);
	~dialog_variables();

private:
    QList<list_variables> *vrb;

private slots:
    void clicked_but();
};

#endif // DIALOG_VARIABLES_H
