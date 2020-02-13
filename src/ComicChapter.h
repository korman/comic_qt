/*
 *
 */

#ifndef COMICCHAPTER_H
#define COMICCHAPTER_H

#include <QObject>
#include <QVector>

class ComicChapter:public QObject
{
    Q_OBJECT
public:
    ComicChapter(QObject* parent = nullptr);

    Q_INVOKABLE int chapterCount();

    QByteArray dataSource(int index);

    bool loadChapter(const QString& path);
    bool openChapter();

    void setIndex(int index) {_index = index;}
    int index() {return _index;}

    void setMaxWidth(int max) {_maxWidth = max;}

    void setChapterName(const QString& name) {_chapterName = name;}
    const QString& chapterName() {return _chapterName;}

    void clearData();

    const QString& filePath(int index);

    void setPath(const QString& path) {_path = path;}

protected:

    int _index;
    QString _path;
    QString _chapterName;

    int _maxWidth;

    QVector<QByteArray> _base64datas;
    QVector<QString> _filePaths;

private:
};

#endif
