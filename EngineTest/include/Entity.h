#ifndef ENTITY_H_
#define ENTITY_H_

#include "Object.h"

#include <QScriptEngine>
#include <QString>

class Entity : public Object
{
	Q_OBJECT

public:
	Entity(QObject* parent = 0);

	Q_PROPERTY(QString meshName READ meshName WRITE setMeshName)

	const QString& meshName(void) const;
	void setMeshName(const QString& name);

private:
	QString mMeshName;
};

Q_SCRIPT_DECLARE_QMETAOBJECT(Entity, QObject*)


#endif //ENTITY_H_