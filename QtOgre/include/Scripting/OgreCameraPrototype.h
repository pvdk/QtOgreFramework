#ifndef OGRECAMERAPROTOTYPE_H
#define OGRECAMERAPROTOTYPE_H

#include <OgreCamera.h>
#include <OgreVector3.h>

#include <QtCore/QObject>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

class OgreCameraPrototype : public QObject, public QScriptable
{
Q_OBJECT
public:
    OgreCameraPrototype(QObject *parent = 0);
    ~OgreCameraPrototype();

public slots:
    QScriptValue valueOf() const;

	void setPosition(const Ogre::Vector3 &vec);

private:
	Ogre::Camera* thisOgreCamera() const;
};


#endif //OGRECAMERAPROTOTYPE_H
