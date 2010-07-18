#ifndef CAMERA_H_
#define CAMERA_H_

#include <QObject>
#include <QScriptEngine>
#include <QVector3D>

class Camera : public QObject
{
	Q_OBJECT

public:
	Camera(QObject * parent = 0);

	Q_PROPERTY(QVector3D direction READ direction WRITE setDirection)
	Q_PROPERTY(float fieldOfView READ fieldOfView WRITE setFieldOfView)
	Q_PROPERTY(QVector3D position READ position WRITE setPosition)
	Q_PROPERTY(QVector3D right READ right WRITE setRight)

	const QVector3D& direction(void) const;
	void setDirection(const QVector3D& direction);

	float fieldOfView(void) const;
	void setFieldOfView(float fieldOfView);

	const QVector3D& position(void) const;
	void setPosition(const QVector3D& position);

	const QVector3D& right(void) const;
	void setRight(const QVector3D& right);

public slots:
		void pitch(float angleInDegrees);

private:
	QVector3D mDirection;
	float mFieldOfView;
	QVector3D mPosition;
	QVector3D mRight;
};

#endif //CAMERA_H_