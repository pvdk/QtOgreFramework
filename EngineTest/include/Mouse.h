#ifndef MOUSE_H_
#define MOUSE_H_

#include <QObject>
#include <QPoint>

class Mouse : public QObject
{
	Q_OBJECT

public:
	Mouse(void);

public slots:
	bool isPressed(int mouseButton);

	void press(int mouseButton);

	void release(int mouseButton);

	const QPoint& pos(void);

	void setPos(const QPoint& pos);

	QPoint computeDelta(void);

	void resetDelta(void);

	void modifyWheelDelta(int wheelDelta);

	int getWheelDelta(void);

	void resetWheelDelta(void);

private:
	Qt::MouseButtons mMouseButtons;
	QPoint mPos;
	QPoint mOldPos;
	int mWheelDelta;
};

#endif //MOUSE_H_