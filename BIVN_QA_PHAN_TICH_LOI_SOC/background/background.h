#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QObject>

class Background : public QObject
{
    Q_OBJECT
public:
    explicit Background(QObject *parent = nullptr);

signals:

};

#endif // BACKGROUND_H
