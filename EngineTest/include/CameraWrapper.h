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

	Q_PROPERTY(QVector3D position READ getPosition WRITE setPosition)
	Q_PROPERTY(QVector3D direction READ getDirection WRITE setDirection)
	Q_PROPERTY(QVector3D right READ getRight)
	Q_PROPERTY(QVector3D up READ getUp)


public slots:
	void moveRelative(const Ogre::Vector3 &vec);

	void yaw (const Ogre::Radian &angle);

	void pitch (const Ogre::Radian &angle);

	Ogre::Radian getFOVy  (void) const;

	void setFOVy (const Ogre::Radian& fovy);

	QVector3D getPosition(void) const;
	void setPosition(const QVector3D& pos);

	QVector3D getDirection(void) const;
	void setDirection(const QVector3D& dir);

	QVector3D getRight(void) const;

	QVector3D getUp(void) const;

private:
	Ogre::Camera* m_pOgreCamera;
};

#endif //CAMERAWRAPPER_H_