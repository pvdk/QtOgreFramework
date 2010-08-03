#ifndef OBJECT_H_
#define OBJECT_H_

#include <QMatrix4x4>
#include <QObject>
#include <QQuaternion>
#include <QScriptEngine>
#include <QVector3D>

class Object : public QObject
{
	Q_OBJECT

public:
	Object(QObject* parent = 0);

	Q_PROPERTY(QVector3D position READ position WRITE setPosition)

	Q_PROPERTY(QVector3D xAxis READ xAxis)
	Q_PROPERTY(QVector3D yAxis READ yAxis)
	Q_PROPERTY(QVector3D zAxis READ zAxis)

	const QVector3D& position(void) const;
	void setPosition(const QVector3D& position);

	const QVector3D xAxis(void) const;
	const QVector3D yAxis(void) const;
	const QVector3D zAxis(void) const;

public slots:
	void translate(const QVector3D & vector);
	void translate(qreal x, qreal y, qreal z);

	void pitch(qreal angleInDegrees);
	void yaw(qreal angleInDegrees);
	void roll(qreal angleInDegrees);

public:
	QVector3D mPosition;
	QQuaternion mOrientation;
};

#endif //OBJECT_H_