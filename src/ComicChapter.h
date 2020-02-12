/*
 *
 */

#ifndef COMICCHAPTER_H
#define COMICCHAPTER_H

#include <QObject>

class ComicChapter:public QObject
{
    Q_OBJECT
public:
    ComicChapter(QObject* parent = nullptr);

    bool loadChapter(const QString& path);

    void setIndex(int index) {_index = index;}
    int index() {return _index;}

    void setChapterName(const QString& name) {_chapterName = name;}
    const QString& chapterName() {return _chapterName;}

    void setPath(const QString& path) {_path = path;}

protected:

    int _index;
    QString _path;
    QString _chapterName;

private:
};

#endif
