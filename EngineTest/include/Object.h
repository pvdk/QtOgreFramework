#ifndef OBJECT_H_
#define OBJECT_H_

#include <QMatrix4x4.h>
#include <QObject>
#include <QScriptEngine>
#include <QVector3D>

class Object : public QObject
{
	Q_OBJECT

public:
	Object(QObject* parent = 0);

	Q_PROPERTY(QVector3D direction READ direction)
	Q_PROPERTY(QVector3D position READ position WRITE setPosition)
	Q_PROPERTY(QVector3D right READ right)

	const QVector3D direction(void) const;

	const QVector3D& position(void) const;
	void setPosition(const QVector3D& position);

	const QVector3D right(void) const;

	const QVector3D up(void) const;

public slots:
		void pitch(float angleInDegrees);
		void yaw(float angleInDegrees);

private:
	QVector3D mDirection;
	QVector3D mPosition;
	QVector3D mRight;
	QMatrix4x4 mRotation;
};

#endif //OBJECT_H_