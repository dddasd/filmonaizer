#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"

#define ApplicationVersion "0.3.25"

#ifdef QT_DEBUG
QFile file;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    //Q_UNUSED(context);
    switch (type) {
        case QtDebugMsg:
            file.write(QString("%1 - Debug: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toUtf8());
            break;
        case QtWarningMsg:
            file.write(QString("%1 - Warning: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toUtf8());
            break;
        case QtCriticalMsg:
            file.write(QString("%1 - Critical: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toUtf8());
            break;
        case QtFatalMsg:
            file.write(QString("%1 - Fatal: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toUtf8());
            file.flush();
            abort();
    }
    file.flush();
}
#endif

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    file.setFileName("debug.log");
    if (!file.exists(file.fileName()))
        file.open(QIODevice::WriteOnly);
    else
        file.open(QIODevice::Append);
    qInstallMessageHandler(myMessageOutput);
#endif

    QApplication a(argc, argv);

    bool portable = true;
    for (int i = 0; i < qApp->arguments().length(); i++)
        if (qApp->arguments()[i] == "-np")
            portable = false;

	a.setApplicationName(QApplication::tr("filmonaizer"));
	a.setApplicationVersion(ApplicationVersion);

    MainWindow wid(portable);
    wid.show();


    int i = a.exec();

    qDebug() << QString(QObject::tr("return code application - %1")).arg(i);

#ifdef QT_DEBUG    
    file.close();
#endif

    return i;
}
