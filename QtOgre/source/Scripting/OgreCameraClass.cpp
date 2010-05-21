#include "Scripting/OgreCameraClass.h"
#include "Scripting/OgreCameraPrototype.h"

#include <QtScript/QScriptClassPropertyIterator>
#include <QtScript/QScriptEngine>

#include <stdlib.h>

Q_DECLARE_METATYPE(Ogre::Camera*)
//Q_DECLARE_METATYPE(Ogre::Camera)
Q_DECLARE_METATYPE(OgreCameraClass*)

OgreCameraClass::OgreCameraClass(QScriptEngine *engine)
    : QObject(engine), QScriptClass(engine)
{
    //qScriptRegisterMetaType<Ogre::Camera>(engine, toScriptValue, fromScriptValue);

    /*x = engine->toStringHandle(QLatin1String("x"));
	y = engine->toStringHandle(QLatin1String("y"));
	z = engine->toStringHandle(QLatin1String("z"));*/

    proto = engine->newQObject(new OgreCameraPrototype(this),
                               QScriptEngine::QtOwnership,
                               QScriptEngine::SkipMethodsInEnumeration
                               | QScriptEngine::ExcludeSuperClassMethods
                               | QScriptEngine::ExcludeSuperClassProperties);
    QScriptValue global = engine->globalObject();
    proto.setPrototype(global.property("Object").property("prototype"));

    /*ctor = engine->newFunction(construct);
    ctor.setData(qScriptValueFromValue(engine, this));*/
}

OgreCameraClass::~OgreCameraClass()
{
}

QScriptClass::QueryFlags OgreCameraClass::queryProperty(const QScriptValue &object,
                                                       const QScriptString &name,
                                                       QueryFlags flags, uint *id)
{
    Ogre::Camera *ba = qscriptvalue_cast<Ogre::Camera*>(object.data());
    if (!ba)
        return 0;
    /*if (name == x)
	{
        return flags;
    }
	if (name == y)
	{
        return flags;
    }
	if (name == z)
	{
        return flags;
    }*/
       return 0;
}

QScriptValue OgreCameraClass::property(const QScriptValue &object,
                                      const QScriptString &name, uint id)
{
    Ogre::Camera *ba = qscriptvalue_cast<Ogre::Camera*>(object.data());
    if (!ba)
        return QScriptValue();
    /*if (name == x)
	{
        return ba->x;
    }
	if (name == y)
	{
        return ba->y;
    }
	if (name == z)
	{
        return ba->z;
    }*/
    return QScriptValue();
}

void OgreCameraClass::setProperty(QScriptValue &object,
                                 const QScriptString &name,
                                 uint id, const QScriptValue &value)
{
    Ogre::Camera *ba = qscriptvalue_cast<Ogre::Camera*>(object.data());
    if (!ba)
        return;
    /*if (name == x)
	{
        ba->x = value.toNumber();
    }
	if (name == y)
	{
        ba->y = value.toNumber();
    }
	if (name == z)
	{
        ba->z = value.toNumber();
    }*/
}

QScriptValue::PropertyFlags OgreCameraClass::propertyFlags(
    const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/)
{
    /*if (name == x)
	{
        return QScriptValue::Undeletable
            | QScriptValue::SkipInEnumeration;
    }
	if (name == y)
	{
        return QScriptValue::Undeletable
            | QScriptValue::SkipInEnumeration;
    }
	if (name == z)
	{
        return QScriptValue::Undeletable
            | QScriptValue::SkipInEnumeration;
    }*/
    return QScriptValue::Undeletable;
}

QString OgreCameraClass::name() const
{
    return QLatin1String("OgreCamera");
}

QScriptValue OgreCameraClass::prototype() const
{
    return proto;
}

/*QScriptValue OgreCameraClass::constructor()
{
    return ctor;
}*/

/*QScriptValue OgreCameraClass::newInstance(int size)
{
	return newInstance(Ogre::Camera());
}*/

/*QScriptValue OgreCameraClass::newInstance(const Ogre::Camera &ba)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(ba));
    return engine()->newObject(this, data);
}*/

/*QScriptValue OgreCameraClass::construct(QScriptContext *ctx, QScriptEngine *)
{
    OgreCameraClass *cls = qscriptvalue_cast<OgreCameraClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    int size = ctx->argument(0).toInt32();
    return cls->newInstance(size);
}*/

/*QScriptValue OgreCameraClass::toScriptValue(QScriptEngine *eng, const Ogre::Camera &ba)
{
    QScriptValue ctor = eng->globalObject().property("OgreCamera");
    OgreCameraClass *cls = qscriptvalue_cast<OgreCameraClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(qVariantFromValue(ba));
    return cls->newInstance(ba);
}*/

/*void OgreCameraClass::fromScriptValue(const QScriptValue &obj, Ogre::Camera &ba)
{
    ba = qscriptvalue_cast<Ogre::Camera>(obj.data());
}*/
