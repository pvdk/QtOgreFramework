#include "Scripting/OgreRadianClass.h"
#include "Scripting/OgreRadianPrototype.h"

#include <QtScript/QScriptClassPropertyIterator>
#include <QtScript/QScriptEngine>

#include <stdlib.h>

OgreRadianClass::OgreRadianClass(QScriptEngine *engine)
    : QObject(engine), QScriptClass(engine)
{
    qScriptRegisterMetaType<Ogre::Radian>(engine, toScriptValue, fromScriptValue);

    valueRadians = engine->toStringHandle(QLatin1String("valueRadians"));
	//y = engine->toStringHandle(QLatin1String("y"));
	//z = engine->toStringHandle(QLatin1String("z"));

    proto = engine->newQObject(new OgreRadianPrototype(this),
                               QScriptEngine::QtOwnership,
                               QScriptEngine::SkipMethodsInEnumeration
                               | QScriptEngine::ExcludeSuperClassMethods
                               | QScriptEngine::ExcludeSuperClassProperties);
    QScriptValue global = engine->globalObject();
    proto.setPrototype(global.property("Object").property("prototype"));

    ctor = engine->newFunction(construct);
    ctor.setData(qScriptValueFromValue(engine, this));
}

OgreRadianClass::~OgreRadianClass()
{
}

QScriptClass::QueryFlags OgreRadianClass::queryProperty(const QScriptValue &object,
                                                       const QScriptString &name,
                                                       QueryFlags flags, uint *id)
{
    Ogre::Radian *ba = qscriptvalue_cast<Ogre::Radian*>(object.data());
    if (!ba)
        return 0;
    if (name == valueRadians)
	{
        return flags;
    }
	/*if (name == y)
	{
        return flags;
    }
	if (name == z)
	{
        return flags;
    }*/
       return 0;
}

QScriptValue OgreRadianClass::property(const QScriptValue &object,
                                      const QScriptString &name, uint id)
{
    Ogre::Radian *ba = qscriptvalue_cast<Ogre::Radian*>(object.data());
    if (!ba)
        return QScriptValue();
    if (name == valueRadians)
	{
        return ba->valueRadians();
    }
	/*if (name == y)
	{
        return ba->y;
    }
	if (name == z)
	{
        return ba->z;
    }*/
    return QScriptValue();
}

void OgreRadianClass::setProperty(QScriptValue &object,
                                 const QScriptString &name,
                                 uint id, const QScriptValue &value)
{
    Ogre::Radian *ba = qscriptvalue_cast<Ogre::Radian*>(object.data());
    if (!ba)
        return;
    if (name == valueRadians)
	{
        *ba = value.toNumber();
    }
	/*if (name == y)
	{
        ba->y = value.toNumber();
    }
	if (name == z)
	{
        ba->z = value.toNumber();
    }*/
}

QScriptValue::PropertyFlags OgreRadianClass::propertyFlags(
    const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/)
{
    if (name == valueRadians)
	{
        return QScriptValue::Undeletable
            | QScriptValue::SkipInEnumeration;
    }
	/*if (name == y)
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

QString OgreRadianClass::name() const
{
    return QLatin1String("OgreRadian");
}

QScriptValue OgreRadianClass::prototype() const
{
    return proto;
}

QScriptValue OgreRadianClass::constructor()
{
    return ctor;
}

QScriptValue OgreRadianClass::newInstance(qreal value)
{
	return newInstance(Ogre::Radian(value));
}

QScriptValue OgreRadianClass::newInstance(const Ogre::Radian &ba)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(ba));
    return engine()->newObject(this, data);
}

QScriptValue OgreRadianClass::construct(QScriptContext *ctx, QScriptEngine *)
{
    OgreRadianClass *cls = qscriptvalue_cast<OgreRadianClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    qreal value = ctx->argument(0).toNumber();
    return cls->newInstance(value);
}

QScriptValue OgreRadianClass::toScriptValue(QScriptEngine *eng, const Ogre::Radian &ba)
{
    QScriptValue ctor = eng->globalObject().property("OgreRadian");
    OgreRadianClass *cls = qscriptvalue_cast<OgreRadianClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(qVariantFromValue(ba));
    return cls->newInstance(ba);
}

void OgreRadianClass::fromScriptValue(const QScriptValue &obj, Ogre::Radian &ba)
{
	ba = qvariant_cast<Ogre::Radian>(obj.data().toVariant());
}
