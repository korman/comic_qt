#include "ComicChapter.h"
#include <QDir>
#include <QDebug>
#include "comic.pb.h"

using namespace pb;

ComicChapter::ComicChapter(QObject *parent):QObject(parent) {}

int ComicChapter::chapterCount()
{
    return _base64datas.length();
}

QByteArray ComicChapter::dataSource(int index)
{
    qDebug() << "DataLen:" << _base64datas.length() << "Get Source : " << index << endl;

    return _base64datas[index];
}

bool ComicChapter::loadChapter(const QString &path)
{
    _path = path;

    QDir dir(_path);

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    QStringList filterList;
    filterList << "*.jpg";   //设置筛选条件

    QFileInfoList list = dir.entryInfoList(filterList);

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        QFile file(fileInfo.path());

        _filePaths.push_back(fileInfo.filePath());
    }

    return true;
}

bool ComicChapter::openChapter()
{
    if (0 == _filePaths.length())
    {
        return false;
    }

    for (const QString& s: _filePaths)
    {
        QFile file(s);

        if (!file.exists())
        {
            qWarning() << s << "not exist" << endl;
            continue;
        }

        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "File Can't Read" << endl;
            continue;
        }

        QByteArray bytes = file.readAll();

        //qDebug() << "Load file: " << s << "Length = " << bytes.size() << endl;

        _base64datas.push_back(bytes);

       // qDebug() << "getData: " << QString(bytes.toBase64()) << endl;
    }

    return true;
}

bool ComicChapter::processPageListEvent(QByteArray array)
{
    PbPageList list;

    if (!list.ParseFromArray(array.data(),array.size()))
    {
        qWarning() << "Parse Error" << endl;
        return false;
    }

    for (int i = 0;i < list.pages_size();i++)
    {
        QString dataString = list.pages(i).data().c_str();

        QByteArray base64Data = QByteArray::fromBase64(dataString.toLatin1());

        _base64datas.push_back(base64Data);
    }

    return true;
}

void ComicChapter::clearData()
{
    _base64datas.clear();
}

const QString &ComicChapter::filePath(int index)
{
    return _filePaths[index];
}
