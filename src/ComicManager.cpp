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

        if (!book->load(fullDir.path()))
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

bool ComicManager::loadBook(int index)
{
    qDebug() << "Will Load Book" << index << endl;
    shared_ptr<ComicBook> book = _books.at(index);

    return true;
}
