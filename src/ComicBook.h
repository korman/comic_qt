/*
 *
 */

#ifndef COMICBOOK_H
#define COMICBOOK_H

#include <QObject>
#include <memory>

#include "ComicChapter.h"

using namespace std;

class ComicBook:public QObject
{
    Q_OBJECT

public:
    typedef QVector<shared_ptr<ComicChapter>> Chapters;

    ComicBook(QObject* parent = nullptr);
    ~ComicBook();

    Q_INVOKABLE const QString& name() {return _name;}
    Q_INVOKABLE bool load(const QString& path);
    Q_INVOKABLE QString chapterName(int index);
    Q_INVOKABLE int chapterCount();

    void setName(const QString& name) {_name = name;}
    void setPath(const QString& path) {_path = path;}

protected:
    bool parseChapter(const QString& path);

    QString _name;
    QString _path;
    Chapters _chapters;

private:
};

#endif
