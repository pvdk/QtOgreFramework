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

	class Mouse : public QObject
	{
		Q_OBJECT
	public slots:
		bool isPressed(int mouseButton)
		{
			//Note - I'd rather pass a Qt::MouseButton in a parameter to this 
			//function and avoid the class, but I had problems registering it
			//with qScriptRegisterMetaType().
			Qt::MouseButton mb = static_cast<Qt::MouseButton>(mouseButton);

			return mMouseButtons & mb;
		}

		void press(int mouseButton)
		{
			//Note - I'd rather pass a Qt::MouseButton in a parameter to this 
			//function and avoid the class, but I had problems registering it
			//with qScriptRegisterMetaType().
			Qt::MouseButton mb = static_cast<Qt::MouseButton>(mouseButton);

			mMouseButtons |= mb;
		}

		void release(int mouseButton)
		{
			//Note - I'd rather pass a Qt::MouseButton in a parameter to this 
			//function and avoid the class, but I had problems registering it
			//with qScriptRegisterMetaType().
			Qt::MouseButton mb = static_cast<Qt::MouseButton>(mouseButton);

			mMouseButtons &= ~mb;
		}

		const QPoint& pos(void)
		{
			return mPos;
		}

		void setPos(const QPoint& pos)
		{
			mPos = pos;
		}

		QPoint computeDelta(void)
		{
			return mPos - mOldPos;
		}

		void resetDelta(void)
		{
			mOldPos = mPos;
		}

		const int wheelPos(void)
		{
			return mWheelPos;
		}

		void setWheelPos(int wheelPos)
		{
			mWheelPos = wheelPos;
		}

		int computeWheelDelta(void)
		{
			return mWheelPos - mOldWheelPos;
		}

		void resetWheelDelta(void)
		{
			mOldWheelPos = mWheelPos;
		}

	private:
		Qt::MouseButtons mMouseButtons;
		QPoint mPos;
		QPoint mOldPos;
		int mWheelPos;
		int mOldWheelPos;
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
		void moveRelative(const Ogre::Vector3 &vec)
		{
			m_pOgreCamera->moveRelative(vec);
		}

		void yaw (const Ogre::Radian &angle)
		{
			m_pOgreCamera->yaw(angle);
		}

		void pitch (const Ogre::Radian &angle)
		{
			m_pOgreCamera->pitch(angle);
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
		MyStruct myStruct;
		QScriptValue cameraPositionScriptValue;
		QScriptValue cameraDirectionScriptValue;
		QScriptValue cameraRightScriptValue;
	};

	
}

Q_DECLARE_METATYPE(QtOgre::MyStruct)

#endif /*DEMOGAMELOGIC_H_*/