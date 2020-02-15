/*
 *
 */

#ifndef REMOTEBOOKLISTDELEGATE_H
#define REMOTEBOOKLISTDELEGATE_H

#include <QObject>

class RemoteBookListDelegate : public QObject
{
    Q_OBJECT
public:
    RemoteBookListDelegate(QObject* parent = nullptr);
    virtual ~RemoteBookListDelegate();
};

#endif
