#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "ComicManager.h"
#include "ComicBook.h"
#include "ComicChapter.h"
#include "ComicPainter.h"
#include "RemoteBookListDelegate.h"

#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>

void logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;

    switch(type)
    {
    case QtDebugMsg:
         text = QString("Debug:");
         break;

    case QtWarningMsg:
         text = QString("Warning:");
         break;

    case QtCriticalMsg:
         text = QString("Critical:");
         break;

    case QtFatalMsg:
        text = QString("Fatal:");
        break;

    case QtInfoMsg:
        text = QString("Info:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QFile file("/Users/kakuhiroshi/code/qt_comic/debug_desktop/babiwawa.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qInstallMessageHandler(logMessage);

    qmlRegisterType<ComicChapter>("Comic.Common",1,0,"ComicChapter");
    qmlRegisterType<ComicBook>("Comic.Common",1,0,"ComicBook");
    qmlRegisterType<ComicPainter>("Comic.Common",1,0,"ComicPainter");
    qmlRegisterType<RemoteDelegate>("Comic.Common",1,0,"RemoteDelegate");

    qmlRegisterSingletonType<ComicManager>("Comic.Common", 1, 0, "ComicMgr", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return ComicManager::instance().get();
    });

    qDebug() << "Start" << endl;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
