#include "CameraWrapper.h"

#include <OgreCamera.h>
#include <OgreMath.h>

void CameraWrapper::setOgreCamera(Ogre::Camera* pOgreCamera)
{
	m_pOgreCamera = pOgreCamera;
}

void CameraWrapper::moveRelative(const Ogre::Vector3 &vec)
{
	m_pOgreCamera->moveRelative(vec);
}

void CameraWrapper::yaw (const Ogre::Radian &angle)
{
	m_pOgreCamera->yaw(angle);
}

void CameraWrapper::pitch (const Ogre::Radian &angle)
{
	m_pOgreCamera->pitch(angle);
}

Ogre::Radian CameraWrapper::getFOVy  (void) const
{
	return m_pOgreCamera->getFOVy();
}

void CameraWrapper::setFOVy (const Ogre::Radian& fovy)
{
	m_pOgreCamera->setFOVy(fovy);
}

QVector3D CameraWrapper::getPosition(void) const
{
	Ogre::Vector3 pos = m_pOgreCamera->getPosition();
	return QVector3D(pos.x, pos.y, pos.z);
}

void CameraWrapper::setPosition(const QVector3D& pos)
{
	m_pOgreCamera->setPosition(pos.x(), pos.y(), pos.z());
}

QVector3D CameraWrapper::getDirection(void) const
{
	Ogre::Vector3 dir = m_pOgreCamera->getDirection();
	return QVector3D(dir.x, dir.y, dir.z);
}

void CameraWrapper::setDirection(const QVector3D& dir)
{
	m_pOgreCamera->setDirection(dir.x(), dir.y(), dir.z());
}

QVector3D CameraWrapper::getRight(void) const
{
	Ogre::Vector3 right = m_pOgreCamera->getRight();
	return QVector3D(right.x, right.y, right.z);
}

QVector3D CameraWrapper::getUp(void) const
{
	Ogre::Vector3 up = m_pOgreCamera->getUp();
	return QVector3D(up.x, up.y, up.z);
}
