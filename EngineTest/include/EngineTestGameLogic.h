#ifndef ENGINETESTGAMELOGIC_H_
#define ENGINETESTGAMELOGIC_H_

#include "CameraWrapper.h"
#include "GameLogic.h"
#include "Log.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <OgreCamera.h>
#include <OgrePrerequisites.h>

#include <QHash>

#include <QTime>

#include <QtScript>
#include <QScriptEngineDebugger>


class StyleSettingsWidget;
	
class EngineTestGameLogic : public QtOgre::GameLogic
{
public:
	EngineTestGameLogic(void);

	void initialise(void);
	void update(void);
	void shutdown(void);

	void onKeyPress(QKeyEvent* event);
	void onKeyRelease(QKeyEvent* event);

	void onMouseMove(QMouseEvent* event);
	void onMousePress(QMouseEvent* event);
	void onMouseRelease(QMouseEvent* event);

	void onWheel(QWheelEvent* event);

	QtOgre::Log* demoLog(void);

	void loadScene(QString filename);

private:
	void initScriptEngine(void);

	Keyboard keyboard;
	Mouse mouse;
	/*QPoint mLastFrameMousePos;
	QPoint mCurrentMousePos;*/

	/*int mLastFrameWheelPos;
	int mCurrentWheelPos;*/
	QTime* mTime;

	int mLastFrameTime;
	int mCurrentTime;

	bool mIsFirstFrame;

	float mCameraSpeed;
		
	StyleSettingsWidget* mStyleSettingsWidget;

	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneManager;
	QtOgre::Log* mDemoLog;

	//Scripting
	QScriptEngine* scriptEngine;
	QString updateScript;
	CameraWrapper cameraWrapper;
	QScriptValue cameraPositionScriptValue;
	QScriptValue cameraDirectionScriptValue;
	QScriptValue cameraRightScriptValue;

	QScriptEngineDebugger debugger;
};

#endif /*ENGINETESTGAMELOGIC_H_*/