#include "Object.h"

Object::Object(QObject * parent)
	:QObject(parent)
{
}

const QQuaternion& Object::orientation(void) const
{
	return mOrientation;
}

void Object::setOrientation(const QQuaternion& orientation)
{
	mOrientation = orientation;
}

const QVector3D& Object::position(void) const
{
	return mPosition;
}

void Object::setPosition(const QVector3D& position)
{
	mPosition = position;
}

const QVector3D Object::xAxis(void) const
{
	QVector3D axis(1,0,0);
	return mOrientation.rotatedVector(axis);
}

const QVector3D Object::yAxis(void) const
{
	QVector3D axis(0,1,0);
	return mOrientation.rotatedVector(axis);
}

const QVector3D Object::zAxis(void) const
{
	QVector3D axis(0,0,1);
	return mOrientation.rotatedVector(axis);
}

void Object::translate(const QVector3D & vector)
{
	mPosition += vector;
}

void Object::translate(qreal x, qreal y, qreal z)
{
	mPosition += QVector3D(x,y,z);
}

void Object::pitch(qreal angleInDegrees)
{

	QQuaternion rotation = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), angleInDegrees);
	mOrientation *= rotation;
}

void Object::yaw(qreal angleInDegrees)
{
	QQuaternion rotation = QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), angleInDegrees);
	mOrientation *= rotation;
}

void Object::roll(qreal angleInDegrees)
{
	QQuaternion rotation = QQuaternion::fromAxisAndAngle(QVector3D(0,0,1), angleInDegrees);
	mOrientation *= rotation;
}