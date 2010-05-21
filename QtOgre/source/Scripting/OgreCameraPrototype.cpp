#include "Scripting/OgreCameraPrototype.h"

#include <QtScript/QScriptEngine>

Q_DECLARE_METATYPE(Ogre::Camera*)

OgreCameraPrototype::OgreCameraPrototype(QObject* parent)
    : QObject(parent)
{
}

OgreCameraPrototype::~OgreCameraPrototype()
{
}

Ogre::Camera* OgreCameraPrototype::thisOgreCamera() const
{
	return qscriptvalue_cast<Ogre::Camera*>(thisObject().data());
}

QScriptValue OgreCameraPrototype::valueOf() const
{
    return thisObject().data();
}

void OgreCameraPrototype::setPosition(const Ogre::Vector3 &vec)
{
	thisOgreCamera()->setPosition(vec);
}