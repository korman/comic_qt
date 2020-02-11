/*
 *
 */

#ifndef COMICBOOK_H
#define COMICBOOK_H

#include <QObject>

class ComicBook:public QObject
{
    Q_OBJECT

public:
    ComicBook(QObject* parent = nullptr);
    ~ComicBook();

    Q_INVOKABLE const QString& name() {return _name;}
    Q_INVOKABLE bool load(const QString& path);

    void setName(const QString& name) {_name = name;}
    void setPath(const QString& path) {_path = path;}

protected:
    bool parseChapter(const QString& path);

    QString _name;
    QString _path;

private:
};

#endif
