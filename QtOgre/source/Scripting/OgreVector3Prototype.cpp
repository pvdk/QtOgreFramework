#include "Scripting/OgreVector3Prototype.h"

#include <QtScript/QScriptEngine>

Q_DECLARE_METATYPE(Ogre::Vector3*)

OgreVector3Prototype::OgreVector3Prototype(QObject* parent)
    : QObject(parent)
{
}

OgreVector3Prototype::~OgreVector3Prototype()
{
}

Ogre::Vector3* OgreVector3Prototype::thisOgreVector3() const
{
	return qscriptvalue_cast<Ogre::Vector3*>(thisObject().data());
}

QScriptValue OgreVector3Prototype::valueOf() const
{
    return thisObject().data();
}

void OgreVector3Prototype::normalise(void)
{
	thisOgreVector3()->normalise();
}