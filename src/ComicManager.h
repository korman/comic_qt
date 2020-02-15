/*
 *
 */

#ifndef COMICMANAGER_H
#define COMICMANAGER_H

#include "ComicBook.h"
#include "RemoteBookListDelegate.h"

#include <QObject>
#include <memory>
#include <QVector>
#include <QNetworkReply>
#include <QNetworkAccessManager>

using namespace std;

class ComicManager:public QObject
{
    Q_OBJECT

public:
    static shared_ptr<ComicManager> instance();

    Q_INVOKABLE bool remoteLoadDir();
    Q_INVOKABLE bool loadDir(const QString& path);
    Q_INVOKABLE int bookCount();
    Q_INVOKABLE QString bookName(int index);
    Q_INVOKABLE bool openBook(int index);
    Q_INVOKABLE ComicBook* currentOpenBook();
    Q_INVOKABLE void setMaxWidth(int max);
    Q_INVOKABLE void setBaseUrl(const QString& url) {_url = url;}
    Q_INVOKABLE int maxWidth() {return _maxWidth;}
    Q_INVOKABLE void setRemoteBookListCallback(RemoteDelegate* remote) {_remoteBookListCallback = remote;}
    Q_INVOKABLE void setRemoteChapterListCallback(RemoteDelegate* remote) {_remoteChapterListCallback = remote;}

public slots:
    void remoteMessageProcess(QNetworkReply* reply);

protected:
    ComicManager(QObject* parent = nullptr);
    ComicManager(const ComicManager&);
    ComicManager& operator=(const ComicManager&);
    ~ComicManager();

    static void destroy(ComicManager* mgr);

    static shared_ptr<ComicManager> _instance;

    bool processBookList(QByteArray bytes);
    bool processChapterList(QByteArray bytes);

    int _maxWidth;
    QVector<shared_ptr<ComicBook>> _books;
    shared_ptr<QNetworkAccessManager> _networkMgr;
    int _currentOpenBookIndex;

    RemoteDelegate* _remoteBookListCallback;
    RemoteDelegate* _remoteChapterListCallback;

    QString _url;
};

#endif
