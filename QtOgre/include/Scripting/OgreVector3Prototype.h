#ifndef OGREVECTOR3PROTOTYPE_H
#define OGREVECTOR3PROTOTYPE_H

#include <OgreVector3.h>

#include <QtCore/QObject>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

class OgreVector3Prototype : public QObject, public QScriptable
{
Q_OBJECT
public:
    OgreVector3Prototype(QObject *parent = 0);
    ~OgreVector3Prototype();

public slots:
    QScriptValue valueOf() const;

	void normalise(void);

private:
	Ogre::Vector3* thisOgreVector3() const;
};


#endif //OGREVECTOR3PROTOTYPE_H
