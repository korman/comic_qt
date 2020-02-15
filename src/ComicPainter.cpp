#include "ComicPainter.h"
#include "ComicManager.h"
#include "ComicChapter.h"

ComicPainter::ComicPainter(QQuickItem *parent):QQuickPaintedItem(parent)
{
    _currentIndex = -1;
}

void ComicPainter::loadPage(int index)
{
    _currentIndex = index;

    shared_ptr<ComicChapter> chapter = ComicManager::instance()->currentOpenBook()->currentChapterPtr();

    QByteArray data = chapter->dataSource(_currentIndex);
    QString name = chapter->filePath(_currentIndex);

    if (!_image.loadFromData(data))
    {
        qWarning() << "Load From Data Error!" << endl;
    }

    double maxWidth = static_cast<double>(ComicManager::instance()->maxWidth());

    double scale = maxWidth / static_cast<double>(_image.width());

    _image = _image.scaled(_image.width() * scale,_image.height() * scale);
}

int ComicPainter::imageWidth()
{
    return _image.width();
}

int ComicPainter::imageHeight()
{
    return _image.height();
}

void ComicPainter::paint(QPainter *painter)
{
    painter->drawImage(QPoint(0,0),_image);
}
