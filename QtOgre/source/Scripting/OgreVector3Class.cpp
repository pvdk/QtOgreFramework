#include "Scripting/OgreVector3Class.h"
#include "Scripting/OgreVector3Prototype.h"

#include <QtScript/QScriptClassPropertyIterator>
#include <QtScript/QScriptEngine>

#include <stdlib.h>

OgreVector3Class::OgreVector3Class(QScriptEngine *engine)
    : QObject(engine), QScriptClass(engine)
{
    qScriptRegisterMetaType<Ogre::Vector3>(engine, toScriptValue, fromScriptValue);

    x = engine->toStringHandle(QLatin1String("x"));
	y = engine->toStringHandle(QLatin1String("y"));
	z = engine->toStringHandle(QLatin1String("z"));

    proto = engine->newQObject(new OgreVector3Prototype(this),
                               QScriptEngine::QtOwnership,
                               QScriptEngine::SkipMethodsInEnumeration
                               | QScriptEngine::ExcludeSuperClassMethods
                               | QScriptEngine::ExcludeSuperClassProperties);
    QScriptValue global = engine->globalObject();
    proto.setPrototype(global.property("Object").property("prototype"));

    ctor = engine->newFunction(construct);
    ctor.setData(qScriptValueFromValue(engine, this));
}

OgreVector3Class::~OgreVector3Class()
{
}

QScriptClass::QueryFlags OgreVector3Class::queryProperty(const QScriptValue &object,
                                                       const QScriptString &name,
                                                       QueryFlags flags, uint *id)
{
    Ogre::Vector3 *ba = qscriptvalue_cast<Ogre::Vector3*>(object.data());
    if (!ba)
        return 0;
    if (name == x)
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
    }
       return 0;
}

QScriptValue OgreVector3Class::property(const QScriptValue &object,
                                      const QScriptString &name, uint id)
{
    Ogre::Vector3 *ba = qscriptvalue_cast<Ogre::Vector3*>(object.data());
    if (!ba)
        return QScriptValue();
    if (name == x)
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
    }
    return QScriptValue();
}

void OgreVector3Class::setProperty(QScriptValue &object,
                                 const QScriptString &name,
                                 uint id, const QScriptValue &value)
{
    Ogre::Vector3 *ba = qscriptvalue_cast<Ogre::Vector3*>(object.data());
    if (!ba)
        return;
    if (name == x)
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
    }
}

QScriptValue::PropertyFlags OgreVector3Class::propertyFlags(
    const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/)
{
    if (name == x)
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
    }
    return QScriptValue::Undeletable;
}

QString OgreVector3Class::name() const
{
    return QLatin1String("OgreVector3");
}

QScriptValue OgreVector3Class::prototype() const
{
    return proto;
}

QScriptValue OgreVector3Class::constructor()
{
    return ctor;
}

QScriptValue OgreVector3Class::newInstance(int size)
{
	return newInstance(Ogre::Vector3());
}

QScriptValue OgreVector3Class::newInstance(const Ogre::Vector3 &ba)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(ba));
    return engine()->newObject(this, data);
}

QScriptValue OgreVector3Class::construct(QScriptContext *ctx, QScriptEngine *)
{
    OgreVector3Class *cls = qscriptvalue_cast<OgreVector3Class*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    int size = ctx->argument(0).toInt32();
    return cls->newInstance(size);
}

QScriptValue OgreVector3Class::toScriptValue(QScriptEngine *eng, const Ogre::Vector3 &ba)
{
    QScriptValue ctor = eng->globalObject().property("OgreVector3");
    OgreVector3Class *cls = qscriptvalue_cast<OgreVector3Class*>(ctor.data());
    if (!cls)
        return eng->newVariant(qVariantFromValue(ba));
    return cls->newInstance(ba);
}

void OgreVector3Class::fromScriptValue(const QScriptValue &obj, Ogre::Vector3 &ba)
{
	ba = qvariant_cast<Ogre::Vector3>(obj.data().toVariant());
}
