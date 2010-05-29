#include "Scripting/OgreRadianPrototype.h"

#include <QtScript/QScriptEngine>

Q_DECLARE_METATYPE(Ogre::Radian*)

OgreRadianPrototype::OgreRadianPrototype(QObject* parent)
    : QObject(parent)
{
}

OgreRadianPrototype::~OgreRadianPrototype()
{
}

Ogre::Radian* OgreRadianPrototype::thisOgreRadian() const
{
	return qscriptvalue_cast<Ogre::Radian*>(thisObject().data());
}

QScriptValue OgreRadianPrototype::valueOf() const
{
    return thisObject().data();
}