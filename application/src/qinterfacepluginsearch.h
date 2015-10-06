#ifndef QINTERFACEPLUGINSEARCH
#define QINTERFACEPLUGINSEARCH

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class INotifyerSearch: public QObject {
public:
    virtual ~INotifyerSearch() {}
    virtual QObject *instance() = 0;

    virtual void signalSearch(QList<QString>,int,QString) = 0;
    virtual void signalPars(int,QString) = 0;
    virtual void signalSmallImage(int,QList<QString>) = 0;
    virtual void signalDownloadImage(QString,int) = 0;
};

class QInterfacePluginSearch
{
public:
    virtual ~QInterfacePluginSearch() {}

    virtual INotifyerSearch* notifyer() = 0;

    virtual void init_plug() = 0;
    virtual void set_proxy(bool proxy, QString host, QString port, QString username, QString password) = 0;

    virtual void result_search_movie(QString) = 0; //Поиск фильма, (строка поиска)
    virtual void result_pars_movie(int,QString) = 0; //Загрузка фильма, и парсинг, (индекс найденного фильма)
    virtual void result_search_small_image(int,QString) = 0; //Поиск картинок для предпросмотра (индекс найденного фильма, папка темп)
    virtual void download_image(QString,QString) = 0; //Загрузить картинку по её (ID, папка темп)
    virtual void download_all_image(QString) = 0; //Загрузить все картинки (которые были найдены в мелких)

    virtual QString result_tags(QString) = 0; //Возврат значение тега

    virtual QList<QString> listTags() = 0; //Список тегов из плагина

    virtual QString getVersion() = 0; //Версия плагина
};

QT_BEGIN_NAMESPACE
#define QInterfacePluginSearch_iid "FilmonaizerSearch"
Q_DECLARE_INTERFACE(QInterfacePluginSearch, QInterfacePluginSearch_iid)
QT_END_NAMESPACE

#endif // QINTERFACEPLUGINSEARCH

