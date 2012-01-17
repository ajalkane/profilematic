#ifndef PLATFORMUTIL_H
#define PLATFORMUTIL_H

#include <QObject>

class PlatformUtil : public QObject
{
    Q_OBJECT
protected:
    PlatformUtil(QObject *parent = 0);
public:
    static PlatformUtil *create();
    virtual ~PlatformUtil();

    virtual void setFlightMode(int flightMode);
};

#endif // PLATFORMUTIL_H
