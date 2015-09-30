#ifndef DIALOG_PLUGINS_H
#define DIALOG_PLUGINS_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DialogPlugins : public QWidget
{
    Q_OBJECT
public:
    explicit DialogPlugins(QList<QString> plug_search, QString cureent_pl_s, QList<QString> plug_media, QString cureent_pl_m, QWidget *parent = 0);

private:
    QGroupBox *gr_search;
    QGroupBox *gr_media;

signals:
    void save_plug(QString,QString);

public slots:
    void click_save();
};

#endif // DIALOG_PLUGINS_H
