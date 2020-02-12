#include "ComicChapter.h"

ComicChapter::ComicChapter(QObject *parent):QObject(parent) {}

bool ComicChapter::loadChapter(const QString &path)
{
    _path = path;

    return true;
}
