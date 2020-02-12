#include "ComicBook.h"
#include <QDir>
#include <QDebug>

ComicBook::ComicBook(QObject *parent):QObject(parent)
{
    _name = "Unknow";
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

QString ComicBook::chapterName(int index)
{
    return _chapters[index]->chapterName();
}

int ComicBook::chapterCount()
{
    return _chapters.size();
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
