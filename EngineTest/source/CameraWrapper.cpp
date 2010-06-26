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