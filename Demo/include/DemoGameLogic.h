#ifndef DEMOGAMELOGIC_H_
#define DEMOGAMELOGIC_H_

#include "ChooseMeshWidget.h"
#include "GameLogic.h"
#include "MainMenu.h"

#include <OgrePrerequisites.h>

#include <QHash>

#include <QTime>

namespace QtOgre
{
	enum KeyStates
	{
		KS_RELEASED,
		KS_PRESSED
	};
	
	class StyleSettingsWidget;
	
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
		QHash<int, KeyStates> mKeyStates;
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
	};
}

#endif /*DEMOGAMELOGIC_H_*/