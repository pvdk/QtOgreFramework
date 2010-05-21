#ifndef OGRECAMERACLASS_H
#define OGRECAMERACLASS_H

#include <QtCore/QObject>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptString>

#include <OgreCamera.h>

class OgreCameraClass : public QObject, public QScriptClass
{
public:
    OgreCameraClass(QScriptEngine *engine);
    ~OgreCameraClass();

    //QScriptValue constructor();

    //QScriptValue newInstance(int size = 0);
	//QScriptValue newInstance(const Ogre::Camera &ba);

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
    //static QScriptValue construct(QScriptContext *ctx, QScriptEngine *eng);

	//static QScriptValue toScriptValue(QScriptEngine *eng, const Ogre::Camera &ba);
    //static void fromScriptValue(const QScriptValue &obj, Ogre::Camera &ba);

    QScriptValue proto;
    //QScriptValue ctor;
};

#endif //OGRECAMERACLASS_H
