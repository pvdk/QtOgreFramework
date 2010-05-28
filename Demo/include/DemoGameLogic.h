#ifndef DEMOGAMELOGIC_H_
#define DEMOGAMELOGIC_H_

#include "ChooseMeshWidget.h"
#include "GameLogic.h"
#include "MainMenu.h"

#include <OgreCamera.h>
#include <OgrePrerequisites.h>

#include <QHash>

#include <QTime>

#include <QtScript>

namespace QtOgre
{
	enum KeyStates
	{
		KS_RELEASED,
		KS_PRESSED
	};
	
	class StyleSettingsWidget;

	class Keyboard : public QObject
	{
		Q_OBJECT
	public slots:
		bool isPressed(int key)
		{
			return mKeyStates[key] == KS_PRESSED;
		}

		void press(int key)
		{
			mKeyStates[key] = KS_PRESSED;
		}

		void release(int key)
		{
			mKeyStates[key] = KS_RELEASED;
		}

	private:
		QHash<int, KeyStates> mKeyStates;
	};

	class CameraWrapper : public QObject
	{
		Q_OBJECT

	public:
		void setOgreCamera(Ogre::Camera* pOgreCamera)
		{
			m_pOgreCamera = pOgreCamera;
		}

	public slots:
		void moveForward()
		{
			Ogre::Vector3 dir = m_pOgreCamera->getDirection();
			dir.normalise();
			dir *= 0.01;
			m_pOgreCamera->setPosition(m_pOgreCamera->getPosition() + dir);
		}

	private:
		Ogre::Camera* m_pOgreCamera;
	};

	struct MyStruct
	{
		int x;
		int y;
	};
	
	class DemoGameLogic : public GameLogic
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

		void onWheel(QWheelEvent* event);

		QtOgre::Log* demoLog(void);

		void loadScene(QString filename);

	private:
		Keyboard keyboard;
		QPoint mLastFrameMousePos;
		QPoint mCurrentMousePos;

		int mLastFrameWheelPos;
		int mCurrentWheelPos;
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
		MyStruct myStruct;
		QScriptValue cameraPositionScriptValue;
		QScriptValue cameraDirectionScriptValue;
		QScriptValue cameraRightScriptValue;
	};

	
}

Q_DECLARE_METATYPE(QtOgre::MyStruct)

#endif /*DEMOGAMELOGIC_H_*/