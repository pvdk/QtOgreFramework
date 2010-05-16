#ifndef OGREVECTOR3CLASS_H
#define OGREVECTOR3CLASS_H

#include <QtCore/QObject>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptString>

#include <OgreVector3.h>

class OgreVector3Class : public QObject, public QScriptClass
{
public:
    OgreVector3Class(QScriptEngine *engine);
    ~OgreVector3Class();

    QScriptValue constructor();

    QScriptValue newInstance(int size = 0);
	QScriptValue newInstance(const Ogre::Vector3 &ba);

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

	static QScriptValue toScriptValue(QScriptEngine *eng, const Ogre::Vector3 &ba);
    static void fromScriptValue(const QScriptValue &obj, Ogre::Vector3 &ba);

    QScriptString x;
	QScriptString y;
	QScriptString z;
    QScriptValue proto;
    QScriptValue ctor;
};

#endif //OGREVECTOR3CLASS_H
