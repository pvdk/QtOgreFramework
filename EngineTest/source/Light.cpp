#include "Light.h"

Light::Light(QObject * parent)
	:QObject(parent)
{
}

const QVector3D& Light::getPosition(void) const
{
	return m_vecPosition;
}

void Light::setPosition(const QVector3D& pos)
{
	m_vecPosition = pos;
}

const QColor& Light::getColour(void) const
{
	return m_colColour;
}

void Light::setColour(const QColor& col)
{
	m_colColour = col;
}