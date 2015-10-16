#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"

#define ApplicationVersion "0.3.25"

#ifdef QT_DEBUG
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QFile file;
    file.setFileName("debug.log");
    if (!file.exists(file.fileName()))
        file.open(QIODevice::WriteOnly);
    else
        file.open(QIODevice::Append);
    switch (type) {
        case QtDebugMsg:
            file.write(QString("%1 - Debug: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toLocal8Bit());
            break;
        case QtWarningMsg:
            file.write(QString("%1 - Warning: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toLocal8Bit());
            break;
        case QtCriticalMsg:
            file.write(QString("%1 - Critical: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toLocal8Bit());
            break;
        case QtFatalMsg:
            file.write(QString("%1 - Fatal: %2 (%3:%4, %5)\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(msg).arg(context.file).arg(context.line).arg(context.function).toLocal8Bit());
            file.flush();
            abort();
    }
    file.flush();
    file.close();
}
#endif

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
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

    qDebug() << QString(QApplication::tr("return code application - %1")).arg(i);

    return i;
}
