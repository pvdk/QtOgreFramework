#include "Camera.h"

#include <QMatrix4x4>

Camera::Camera(QObject * parent)
	:QObject(parent)
{
	mPosition = QVector3D(0,0,0);
	mDirection = QVector3D(0,0,-1);
	mRight = QVector3D(1,0,0);
	mFieldOfView = 1.0;
}

const QVector3D& Camera::direction(void) const
{
	return mDirection;
}

void Camera::setDirection(const QVector3D& direction)
{
	mDirection = direction;
}

float Camera::fieldOfView(void) const
{
	return mFieldOfView;
}

void Camera::setFieldOfView(float fieldOfView)
{
	mFieldOfView = fieldOfView;
}

const QVector3D& Camera::position(void) const
{
	return mPosition;
}

void Camera::setPosition(const QVector3D& position)
{
	mPosition = position;
}

const QVector3D& Camera::right(void) const
{
	return mRight;
}

void Camera::setRight(const QVector3D& right)
{
	mRight = right;
}

void  Camera::pitch(float angleInDegrees)
{
	QMatrix4x4 transform;
	transform.rotate(angleInDegrees, mRight);

	mDirection = mDirection * transform;
}