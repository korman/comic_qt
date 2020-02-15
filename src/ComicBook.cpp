#include "ComicBook.h"
#include <QDir>
#include <QDebug>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "comic.pb.h"
#include "ComicManager.h"
#include "MessageDefine.h"

using namespace pb;

ComicBook::ComicBook(QObject *parent):QObject(parent)
{
    _name = "Unknow";
    _currentChapterIndex = -1;
    _id = -1;
}

ComicBook::~ComicBook()
{

}

bool ComicBook::load(const QString &path)
{
    _path = path;

    if (!parseChapter(path))
    {
        qWarning() << "Parse Chapter Failed!" << endl;
        return false;
    }

    return true;
}

bool ComicBook::loadRemoteBookInfo()
{
    return true;
}

bool ComicBook::openChapter(int index)
{
    qDebug() << "Open Chapter:" << index << endl;

    _currentChapterIndex = index;

    shared_ptr<QNetworkAccessManager> mgr = ComicManager::instance()->networkManager();

    if (nullptr == mgr)
    {
        return false;
    }

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Content-Type","application/json");
    request.setRawHeader("Accept","text/json,*/*;q=0.5");

    QString url = ComicManager::instance()->baseUrl() + "pagelist";
    url += "?msgId=" + QString::number(MSG_PAGELIST);
    url += "&book_id=" + QString::number(_id);
    url += "&chapter_id=" + QString::number(index);

    request.setUrl(QUrl(url));

    mgr->get(request);

    return _chapters[index]->openChapter();
}

QString ComicBook::chapterName(int index)
{
    return _chapters[index]->chapterName();
}

int ComicBook::chapterCount()
{
    return _chapters.size();
}

ComicChapter *ComicBook::currentChapter()
{
    return _chapters[_currentChapterIndex].get();
}

bool ComicBook::nextPage()
{
    if (nullptr == nextChapterPtr())
    {
        return false;
    }

    return true;
}

bool ComicBook::prePage()
{
    if (nullptr == preChapterPtr())
    {
        return false;
    }

    return true;
}

shared_ptr<ComicChapter> ComicBook::currentChapterPtr()
{
    return _chapters[_currentChapterIndex];
}

shared_ptr<ComicChapter> ComicBook::nextChapterPtr()
{
    if (_chapters.size() <= _currentChapterIndex + 1)
    {
        qWarning() << "Out range page" << endl;
        return nullptr;
    }

    _chapters[_currentChapterIndex]->clearData();

    _currentChapterIndex++;

    openChapter(_currentChapterIndex);

    return _chapters[_currentChapterIndex];
}

shared_ptr<ComicChapter> ComicBook::preChapterPtr()
{
    if (0 > _currentChapterIndex - 1)
    {
        qWarning() << "Out range page" << endl;
        return nullptr;
    }

    _chapters[_currentChapterIndex]->clearData();

    _currentChapterIndex--;

    openChapter(_currentChapterIndex);

    return _chapters[_currentChapterIndex];
}

void ComicBook::setMaxWidth(int max)
{

}

bool ComicBook::processChapterListEvent(QByteArray array)
{
    PbChapterList list;

    if (!list.ParseFromArray(array.data(),array.size()))
    {
        qWarning() << "Parse Error" << endl;
        return false;
    }

    for (int i = 0;i < list.chapters_size();i++)
    {
        shared_ptr<ComicChapter> chapter = shared_ptr<ComicChapter>(new ComicChapter);
        chapter->setIndex(i);
        chapter->setChapterName(QString::fromStdString(list.chapters(i).name()));

//        if (!chapter->loadChapter(fullDir.filePath()))
//        {
//            continue;
//        }

        _chapters.push_back(chapter);
    }

    return true;
}

bool ComicBook::processPageListEvent(QByteArray array)
{
    return _chapters[_currentChapterIndex]->processPageListEvent(array);
}

bool ComicBook::parseChapter(const QString &path)
{
    qDebug() << "Parse this path:" << path << endl;

    QDir dir(path);

    dir.setFilter(QDir::Dirs);

    foreach(QFileInfo fullDir, dir.entryInfoList())
    {
        if(fullDir.fileName() == "." || fullDir.fileName() == "..")
        {
            continue;
        }

  //      qDebug() << "Chapter Name:" << fullDir.fileName().toInt() << endl;

        shared_ptr<ComicChapter> chapter = shared_ptr<ComicChapter>(new ComicChapter);
        chapter->setIndex(fullDir.fileName().toInt());
        chapter->setChapterName(fullDir.fileName());

        if (!chapter->loadChapter(fullDir.filePath()))
        {
            continue;
        }

        _chapters.push_back(chapter);
    }

    return true;
}
