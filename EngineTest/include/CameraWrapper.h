#ifndef CAMERAWRAPPER_H_
#define CAMERAWRAPPER_H_

#include <OgrePrerequisites.h>
#include <OgreMath.h>

#include <QObject>
#include <QVector3D>

class CameraWrapper : public QObject
{
	Q_OBJECT

public:
	void setOgreCamera(Ogre::Camera* pOgreCamera);

public slots:
	void moveRelative(const Ogre::Vector3 &vec);

	void yaw (const Ogre::Radian &angle);

	void pitch (const Ogre::Radian &angle);

	Ogre::Radian getFOVy  (void) const;

	void setFOVy (const Ogre::Radian& fovy);

	QVector3D getPosition(void);
	void setPosition(const QVector3D& pos);

	QVector3D getDirection(void);
	void setDirection(const QVector3D& pos);

private:
	Ogre::Camera* m_pOgreCamera;
};

#endif //CAMERAWRAPPER_H_