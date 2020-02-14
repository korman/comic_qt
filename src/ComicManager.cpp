#include "ComicManager.h"
#include <QDir>
#include <QDebug>
#include <QNetworkRequest>
#include <QEventLoop>

shared_ptr<ComicManager> ComicManager::_instance = shared_ptr<ComicManager>(new ComicManager,ComicManager::destroy);

shared_ptr<ComicManager> ComicManager::instance()
{
    return _instance;
}

bool ComicManager::remoteLoadDir(const QString &url)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Content-Type","application/json");
    request.setRawHeader("Accept","text/json,*/*;q=0.5");

    request.setUrl(QUrl(url));

    QNetworkReply* reply = _networkMgr->get(request);
    QMetaObject::Connection connRet = QObject::connect(_networkMgr.get(), SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));

    return true;
}

ComicManager::ComicManager(QObject *parent):QObject(parent)
{
    _networkMgr = shared_ptr<QNetworkAccessManager>(new QNetworkAccessManager(this));
    _currentOpenBookIndex = -1;
    _maxWidth = 0;
}

ComicManager::~ComicManager()
{
}

void ComicManager::destroy(ComicManager *mgr)
{

}

bool ComicManager::loadDir(const QString &path)
{
    qDebug() << path << endl;

    QDir dir(path);

    dir.setFilter(QDir::Dirs);

    foreach(QFileInfo fullDir, dir.entryInfoList())
    {
        if(fullDir.fileName() == "." || fullDir.fileName() == "..")
        {
            continue;
        }

        qDebug() << "FileName:" << fullDir.fileName() << endl;

        shared_ptr<ComicBook> book = shared_ptr<ComicBook>(new ComicBook);
        book->setName(fullDir.fileName());

        if (!book->load(fullDir.filePath()))
        {
            continue;
        }

        _books.push_back(book);
    }

    return true;
}

int ComicManager::bookCount()
{
    return _books.count();
}

QString ComicManager::bookName(int index)
{
    return _books[index]->name();
}

bool ComicManager::openBook(int index)
{
    _currentOpenBookIndex = index;
    qDebug() << "Will Load Book" << index << endl;
    shared_ptr<ComicBook> book = _books.at(index);

    return true;
}

ComicBook *ComicManager::currentOpenBook()
{
    return _books[_currentOpenBookIndex].get();
}

void ComicManager::setMaxWidth(int max)
{
    _maxWidth = max;

    for (auto v : _books)
    {
        v->setMaxWidth(max);
    }
}

void ComicManager::requestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"request protobufHttp handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "request protobufHttp found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
    }
    else
    {
        qDebug() << "request protobufHttp NoError";
    }
}
