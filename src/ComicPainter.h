/*
 *
 */

#ifndef COMICPAINTER_H
#define COMICPAINTER_H

#include <QQuickPaintedItem>
#include <QImage>
#include <memory>
#include <QPainter>

using namespace std;

class ComicPainter:public QQuickPaintedItem
{
    Q_OBJECT

public:
    ComicPainter(QQuickItem *parent = nullptr);

    Q_INVOKABLE void loadPage(int index);
    Q_INVOKABLE int imageWidth();
    Q_INVOKABLE int imageHeight();

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);

protected:

    int _currentIndex;

    QImage _image;

private:
};

#endif
