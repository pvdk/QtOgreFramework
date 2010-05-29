#ifndef OGRERADIANPROTOTYPE_H
#define OGRERADIANPROTOTYPE_H

#include <OgreMath.h>

#include <QtCore/QObject>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

class OgreRadianPrototype : public QObject, public QScriptable
{
Q_OBJECT
public:
    OgreRadianPrototype(QObject *parent = 0);
    ~OgreRadianPrototype();

public slots:
    QScriptValue valueOf() const;

private:
	Ogre::Radian* thisOgreRadian() const;
};


#endif //OGRERADIANPROTOTYPE_H
