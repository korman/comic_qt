#include "ComicManager.h"
#include <QDir>
#include <QDebug>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include "MessageDefine.h"
#include "comic.pb.h"

using namespace pb;

shared_ptr<ComicManager> ComicManager::_instance = shared_ptr<ComicManager>(new ComicManager,ComicManager::destroy);

shared_ptr<ComicManager> ComicManager::instance()
{
    return _instance;
}

bool ComicManager::remoteLoadDir()
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Content-Type","application/json");
    request.setRawHeader("Accept","text/json,*/*;q=0.5");

    request.setUrl(QUrl(_url + "booklist" + "?msgId=" + QString::number(MSG_BOOKLIST)));

    _networkMgr->get(request);

    return true;
}

ComicManager::ComicManager(QObject *parent):QObject(parent)
{
    _networkMgr = shared_ptr<QNetworkAccessManager>(new QNetworkAccessManager(this));
    QMetaObject::Connection connRet = QObject::connect(_networkMgr.get(), SIGNAL(finished(QNetworkReply*)), this, SLOT(remoteMessageProcess(QNetworkReply*)));

    _currentOpenBookIndex = -1;
    _maxWidth = 0;
    _remoteBookListCallback = nullptr;
    _remotePageListCallback = nullptr;
    _remoteChapterListCallback = nullptr;
}

ComicManager::~ComicManager()
{
}

void ComicManager::destroy(ComicManager *mgr)
{

}

bool ComicManager::processBookList(QByteArray bytes)
{
    PbBookList list;

    if (!list.ParseFromArray(bytes.data(),bytes.size()))
    {
        qWarning() << "Parse Error" << endl;
        return false;
    }

    for (int i = 0;i < list.books_size();i++)
    {
        qDebug() << "Name: " << QString::fromStdString(list.books(i).name()) << endl;

        shared_ptr<ComicBook> book = shared_ptr<ComicBook>(new ComicBook);
        book->setName(QString::fromStdString(list.books(i).name()));
        book->setId(i);

        _books.push_back(book);
    }

    if (_remoteBookListCallback)
    {
        emit _remoteBookListCallback->requestMessage();
    }

    return true;
}

bool ComicManager::processChapterList(QByteArray bytes)
{
    shared_ptr<ComicBook> book = _books[_currentOpenBookIndex];

    if (!book->processChapterListEvent(bytes))
    {
        return false;
    }

    if (_remoteChapterListCallback)
    {
        emit _remoteChapterListCallback->requestMessage();
    }

    return true;
}

bool ComicManager::processPageList(QByteArray bytes)
{
    shared_ptr<ComicBook> book = _books[_currentOpenBookIndex];

    if (!book->processPageListEvent(bytes))
    {
        return false;
    }

    if (_remotePageListCallback)
    {
        emit _remotePageListCallback->requestMessage();
    }

    return true;
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

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Content-Type","application/json");
    request.setRawHeader("Accept","text/json,*/*;q=0.5");

    request.setUrl(QUrl(_url + "chapterlist?msgId=" + QString::number(MSG_CHAPTERLIST) + "&id=" + QString::number(index)));

    _networkMgr->get(request);

    return book->loadRemoteBookInfo();
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

void ComicManager::remoteMessageProcess(QNetworkReply *reply)
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

    QByteArray a = reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(a, &jsonError));

    if (QJsonParseError::NoError != jsonError.error)
    {
        qWarning() << "Parse Json Failed" << endl;
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    if (!rootObj.contains("pb_data"))
    {
        qWarning() << "Warning: message not contains pb_data" << endl;
        return;
    }

    int msgId = rootObj.value("msg_id").toInt();
    QString pbData = rootObj.value("pb_data").toString();
    QByteArray array = QByteArray::fromBase64(pbData.toLatin1());

    switch (msgId)
    {
    case MSG_BOOKLIST:
    {
        if (!processBookList(array))
        {
            qWarning() << "Process Book List Failed!" << endl;
            return;
        }
    }
        break;
    case MSG_CHAPTERLIST:
    {
        if (!processChapterList(array))
        {
            qWarning() << "Process Chapter List Failed!" << endl;
        }
    }
        break;
    case MSG_PAGELIST:
    {
        if (!processPageList(array))
        {
            qWarning() << "Process Page List Failed!" << endl;
        }
    }
        break;
    default:
    {
        qWarning() << "Can't process this msgId" << msgId << endl;
        break;
    }
    }
}
