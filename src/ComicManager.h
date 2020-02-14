/*
 *
 */

#ifndef COMICMANAGER_H
#define COMICMANAGER_H

#include "ComicBook.h"

#include <QObject>
#include <memory>
#include <QVector>

using namespace std;

class ComicManager:public QObject
{
    Q_OBJECT

public:
    static shared_ptr<ComicManager> instance();

    Q_INVOKABLE bool loadDir(const QString& path);
    Q_INVOKABLE int bookCount();
    Q_INVOKABLE QString bookName(int index);
    Q_INVOKABLE bool openBook(int index);
    Q_INVOKABLE ComicBook* currentOpenBook();
    Q_INVOKABLE void setMaxWidth(int max);
    Q_INVOKABLE int maxWidth() {return _maxWidth;}

protected:
    ComicManager(QObject* parent = nullptr);
    ComicManager(const ComicManager&);
    ComicManager& operator=(const ComicManager&);
    ~ComicManager();

    static void destroy(ComicManager* mgr);

    static shared_ptr<ComicManager> _instance;

    int _maxWidth;
    QVector<shared_ptr<ComicBook>> _books;
    int _currentOpenBookIndex;
};

#endif
