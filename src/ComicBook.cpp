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

bool ComicBook::parseChapter(const QString &path)
{
    QDir dir(path);

    dir.setFilter(QDir::Dirs);

    foreach(QFileInfo fullDir, dir.entryInfoList())
    {
        if(fullDir.fileName() == "." || fullDir.fileName() == "..")
        {
            continue;
        }

        qDebug() << "FileName:" << fullDir.fileName().toInt() << endl;
    }

    return true;
}
