#include "Mouse.h"

Mouse::Mouse(void)
	:mWheelDelta(0)
{
}

bool Mouse::isPressed(int mouseButton)
{
	//Note - I'd rather pass a Qt::MouseButton in a parameter to this 
	//function and avoid the class, but I had problems registering it
	//with qScriptRegisterMetaType().
	Qt::MouseButton mb = static_cast<Qt::MouseButton>(mouseButton);

	return mMouseButtons & mb;
}

void Mouse::press(int mouseButton)
{
	//Note - I'd rather pass a Qt::MouseButton in a parameter to this 
	//function and avoid the class, but I had problems registering it
	//with qScriptRegisterMetaType().
	Qt::MouseButton mb = static_cast<Qt::MouseButton>(mouseButton);

	mMouseButtons |= mb;
}

void Mouse::release(int mouseButton)
{
	//Note - I'd rather pass a Qt::MouseButton in a parameter to this 
	//function and avoid the class, but I had problems registering it
	//with qScriptRegisterMetaType().
	Qt::MouseButton mb = static_cast<Qt::MouseButton>(mouseButton);

	mMouseButtons &= ~mb;
}

const QPoint& Mouse::pos(void)
{
	return mPos;
}

void Mouse::setPos(const QPoint& pos)
{
	mPos = pos;
}

QPoint Mouse::computeDelta(void)
{
	return mPos - mOldPos;
}

void Mouse::resetDelta(void)
{
	mOldPos = mPos;
}

void Mouse::modifyWheelDelta(int wheelDelta)
{
	mWheelDelta += wheelDelta;
}

int Mouse::getWheelDelta(void)
{
	return mWheelDelta;
}

void Mouse::resetWheelDelta(void)
{
	mWheelDelta = 0;
}