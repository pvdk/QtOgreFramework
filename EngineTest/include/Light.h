#ifndef LIGHT_H_
#define LIGHT_H_

#include <QColor>
#include <QObject>
#include <QScriptEngine>
#include <QVector3D>

class Light : public QObject
{
	Q_OBJECT

public:
	Light(QObject * parent = 0);

	Q_PROPERTY(QVector3D position READ getPosition WRITE setPosition)
	Q_PROPERTY(QColor colour READ getColour WRITE setColour)

	const QVector3D& getPosition(void) const;
	void setPosition(const QVector3D& pos);

	const QColor& getColour(void) const;
	void setColour(const QColor& col);

private:
	QVector3D m_vecPosition;
	QColor m_colColour;
};

Q_SCRIPT_DECLARE_QMETAOBJECT(Light, QObject*)


#endif //LIGHT_H_