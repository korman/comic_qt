/*
 *
 */

#ifndef REMOTEBOOKLISTDELEGATE_H
#define REMOTEBOOKLISTDELEGATE_H

#include <QObject>

class RemoteDelegate : public QObject
{
    Q_OBJECT
public:
    RemoteDelegate(QObject* parent = nullptr);
    virtual ~RemoteDelegate();

signals:
    void requestMessage();
};

#endif
