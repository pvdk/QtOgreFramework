#ifndef OGRERADIANCLASS_H
#define OGRERADIANCLASS_H

#include <QtCore/QObject>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptString>

#include <OgreMath.h>

class OgreRadianClass : public QObject, public QScriptClass
{
public:
    OgreRadianClass(QScriptEngine *engine);
    ~OgreRadianClass();

    QScriptValue constructor();

    QScriptValue newInstance(qreal value = 0.0);
	QScriptValue newInstance(const Ogre::Radian &ba);

    QueryFlags queryProperty(const QScriptValue &object,
                             const QScriptString &name,
                             QueryFlags flags, uint *id);

    QScriptValue property(const QScriptValue &object,
                          const QScriptString &name, uint id);

    void setProperty(QScriptValue &object, const QScriptString &name,
                     uint id, const QScriptValue &value);

    QScriptValue::PropertyFlags propertyFlags(
        const QScriptValue &object, const QScriptString &name, uint id);

    QString name() const;

    QScriptValue prototype() const;

private:
    static QScriptValue construct(QScriptContext *ctx, QScriptEngine *eng);

	static QScriptValue toScriptValue(QScriptEngine *eng, const Ogre::Radian &ba);
    static void fromScriptValue(const QScriptValue &obj, Ogre::Radian &ba);

    QScriptString valueRadians;
	//QScriptString y;
	//QScriptString z;
    QScriptValue proto;
    QScriptValue ctor;
};

Q_DECLARE_METATYPE(Ogre::Radian*)
Q_DECLARE_METATYPE(Ogre::Radian)
Q_DECLARE_METATYPE(OgreRadianClass*)

#endif //OGRERADIANCLASS_H
