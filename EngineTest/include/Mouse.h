#ifndef MOUSE_H_
#define MOUSE_H_

#include <QObject>
#include <QPoint>

class Mouse : public QObject
{
	Q_OBJECT

public:
	Mouse(QObject * parent = 0);

	Q_PROPERTY(QPoint position READ position WRITE setPosition)
	Q_PROPERTY(QPoint previousPosition READ previousPosition)

public slots:
	bool isPressed(int mouseButton);

	void press(int mouseButton);

	void release(int mouseButton);

	const QPoint& position(void);

	void setPosition(const QPoint& pos);

	const QPoint& previousPosition(void);

	void resetDelta(void);

	void modifyWheelDelta(int wheelDelta);

	int getWheelDelta(void);

	void resetWheelDelta(void);

private:
	Qt::MouseButtons mMouseButtons;
	QPoint mPosition;
	QPoint mPreviousPosition;
	int mWheelDelta;
};

#endif //MOUSE_H_