#include "ComicManager.h"
#include <QDir>
#include <QDebug>

shared_ptr<ComicManager> ComicManager::_instance = shared_ptr<ComicManager>(new ComicManager,ComicManager::destroy);

shared_ptr<ComicManager> ComicManager::instance()
{
    return _instance;
}

ComicManager::ComicManager(QObject *parent):QObject(parent)
{
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
    qDebug() << "get current:" << _currentOpenBookIndex << endl;

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
