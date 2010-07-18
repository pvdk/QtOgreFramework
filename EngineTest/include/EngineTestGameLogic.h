#ifndef ENGINETESTGAMELOGIC_H_
#define ENGINETESTGAMELOGIC_H_

#include "CameraWrapper.h"
#include "GameLogic.h"
#include "Globals.h"
#include "Keyboard.h"
#include "Light.h"
#include "Log.h"
#include "Mouse.h"
#include "ObjectStore.h"
#include "ScriptEditorWidget.h"

#include <OgreCamera.h>
#include <OgrePrerequisites.h>

#include <QHash>

#include <QTime>

#include <QtScript>
#include <QScriptEngineDebugger>


class StyleSettingsWidget;
	
class EngineTestGameLogic : public QObject, public QtOgre::GameLogic
{
	Q_OBJECT
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

private slots:
	void startScriptingEngine(void);
	void stopScriptingEngine(void);

private:
	void initScriptEngine(void);
	void initScriptEnvironment(void);

	Keyboard keyboard;
	Mouse* mouse;
	/*QPoint mLastFrameMousePos;
	QPoint mCurrentMousePos;*/

	/*int mLastFrameWheelPos;
	int mCurrentWheelPos;*/
	QTime* mTime;

	//int mLastFrameTime;
	//int mCurrentTime;
	QVariant mCurrentTimeVariant;

	bool mIsFirstFrame;

	quint32 m_uTimeSinceStart;

	float mCameraSpeed;
		
	StyleSettingsWidget* mStyleSettingsWidget;

	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneManager;
	QtOgre::Log* mDemoLog;

	//Scripting
	QScriptEngine* scriptEngine;
	CameraWrapper cameraWrapper;
	QScriptValue cameraPositionScriptValue;
	QScriptValue cameraDirectionScriptValue;
	QScriptValue cameraRightScriptValue;

	QScriptEngineDebugger debugger;

	ScriptEditorWidget* m_pScriptEditorWidget;

	bool m_bRunScript;

	QHash<QString, Light*> m_Lights;

	ObjectStore mObjectStore;

	QString mInitialiseScript;

	Globals* mGlobals;
};

#endif /*ENGINETESTGAMELOGIC_H_*/