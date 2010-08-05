#include "Entity.h"

Entity::Entity(QObject* parent)
	:Object(parent)
{
}

const QString& Entity::meshName(void) const
{
	return mMeshName;
}

void Entity::setMeshName(const QString& name)
{
	mMeshName = name;
}