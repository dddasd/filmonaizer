#ifndef QINTERFACEPLUGINMOVIE
#define QINTERFACEPLUGINMOVIE

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class INotifyerMovie: public QObject {
public:
    virtual ~INotifyerMovie() {}
    virtual QObject *instance() = 0;

    virtual void signalPars(int,QString) = 0;
};

class QInterfacePluginMovie
{
public:
    virtual ~QInterfacePluginMovie() {}

    virtual INotifyerMovie* notifyer() = 0;

    virtual void init_plug() = 0;

    virtual void result_movie(QString) = 0; //Парсер фильма, аргумент - путь до файла

    virtual QString result_tags(QString) = 0; //Возврат значение тега

    virtual QList<QString> listTags() = 0; //Список тегов из плагина
};

QT_BEGIN_NAMESPACE
#define QInterfacePluginMovie_iid "FilmonaizerMovie"
Q_DECLARE_INTERFACE(QInterfacePluginMovie, QInterfacePluginMovie_iid)
QT_END_NAMESPACE

#endif // QINTERFACEPLUGINMOVIE

