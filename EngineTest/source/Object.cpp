#include "Object.h"

#include <QMatrix4x4>

Object::Object(QObject * parent)
	:QObject(parent)
{
	mPosition = QVector3D(0,0,0);


	mDirection = QVector3D(0,0,-1);
	mRight = QVector3D(1,0,0);

	mRotation.setToIdentity();

	//mTransformation.setToIdentity();
}

const QVector3D Object::direction(void) const
{
	QVector4D z(0,0,1,0);
	return (mRotation * z).toVector3D();
	//return mDirection;
}

/*void Object::setDirection(const QVector3D& direction)
{
	mDirection = direction;
}*/

const QVector3D& Object::position(void) const
{
	return mPosition;
	//return mTransformation.row(3).toVector3DAffine();
}

void Object::setPosition(const QVector3D& position)
{
	mPosition = position;
	/*QVector4D posTemp = position.toVector4D();
	posTemp.setW(mTransformation.row(3).w());
	mTransformation.setRow(3, posTemp);*/
}

const QVector3D Object::right(void) const
{
	QVector4D x(1,0,0,0);
	return (mRotation * x).toVector3D();
	//return mRight;
}

/*void Object::setRight(const QVector3D& right)
{
	mRight = right;
}*/

void Object::pitch(float angleInDegrees)
{
	/*QMatrix4x4 transform;
	transform.rotate(angleInDegrees, mRight);

	mDirection = mDirection * transform;*/

	mRotation.rotate(angleInDegrees, right());
}

void Object::yaw(float angleInDegrees)
{
	/*QMatrix4x4 transform;
	transform.rotate(angleInDegrees, mRight);

	mDirection = mDirection * transform;*/

	mRotation.rotate(angleInDegrees, up());
}

const QVector3D Object::up(void) const
{
	QVector4D y(0,1,0,0);
	return (mRotation * y).toVector3D();
	//return mRight;
}