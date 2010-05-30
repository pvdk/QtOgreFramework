#ifndef DEMOGAMELOGIC_H_
#define DEMOGAMELOGIC_H_

#include "CameraWrapper.h"
#include "ChooseMeshWidget.h"
#include "GameLogic.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MainMenu.h"

#include <OgreCamera.h>
#include <OgrePrerequisites.h>

#include <QHash>

#include <QTime>

#include <QtScript>


class StyleSettingsWidget;
	
class DemoGameLogic : public QtOgre::GameLogic
{
public:
	DemoGameLogic(void);

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

	ChooseMeshWidget* mChooseMeshWidget;

	MainMenu* mMainMenu;
		
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
};

#endif /*DEMOGAMELOGIC_H_*/