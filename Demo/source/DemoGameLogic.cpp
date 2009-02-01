#include "DemoGameLogic.h"
#include "DotSceneHandler.h"
#include "MainMenu.h"

#include "LogManager.h"
#include "OgreWidget.h"

#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QSettings>

namespace QtOgre
{
	DemoGameLogic::DemoGameLogic(void)
		:GameLogic()
	{
	}

	void DemoGameLogic::initialise(void)
	{
		mDemoLog = mApplication->createLog("Demo");

		mDemoLog->logMessage("A demonstration debug message", LL_DEBUG);
		mDemoLog->logMessage("A demonstration info message", LL_INFO);
		mDemoLog->logMessage("A demonstration warning message", LL_WARNING);
		mDemoLog->logMessage("A demonstration error message", LL_ERROR);

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/models", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/textures", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/materials", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		// Create the generic scene manager
		mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "GenericSceneManager");

		//Set up scene
		loadScene("media\\scenes\\test.scene");

		//mApplication->ogreRenderWindow()->addViewport(mCamera)->setBackgroundColour(Ogre::ColourValue::Black);

		mSceneManager->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

		//Create the MainMenu
		mMainMenu = new MainMenu(qApp, qApp->mainWidget());

		//Create widget to choose between models
		//mChooseMeshWidget = new ChooseMeshWidget(mJaiquaEntity, mRobotEntity, qApp->mainWidget());
		//mChooseMeshWidget->setWindowOpacity(qApp->settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());
		//mChooseMeshWidget->move(qApp->mainWidget()->geometry().left() + qApp->mainWidget()->geometry().width() - mChooseMeshWidget->frameGeometry().width() - 10, qApp->mainWidget()->geometry().top() + 10);
		//mChooseMeshWidget->show();

		mTime = new QTime;
		mTime->start();

		mIsFirstFrame = true;

		mCameraSpeed = 10.0;

		

		for (Ogre::SceneManager::MovableObjectIterator moi = mSceneManager->getMovableObjectIterator("Entity"); moi.hasMoreElements(); moi.moveNext())
		{
			Ogre::Entity *entity = static_cast<Ogre::Entity*>(moi.peekNextValue());

			Ogre::AnimationStateSet* animationStateSet = entity->getAllAnimationStates();		
			if(animationStateSet && animationStateSet->hasAnimationState("Walk"))
			{
				Ogre::AnimationState* walkAnimationState = animationStateSet->getAnimationState("Walk");
				walkAnimationState->setLoop(true);
				walkAnimationState->setEnabled(true);
			}
		}
	}

	void DemoGameLogic::update(void)
	{
		mLastFrameTime = mCurrentTime;
		mCurrentTime = mTime->elapsed();

		float timeElapsedInSeconds = (mCurrentTime - mLastFrameTime) / 1000.0f;

		for (Ogre::SceneManager::MovableObjectIterator moi = mSceneManager->getMovableObjectIterator("Entity"); moi.hasMoreElements(); moi.moveNext())
		{
			Ogre::Entity *entity = static_cast<Ogre::Entity*>(moi.peekNextValue());

			Ogre::AnimationStateSet* animationStateSet = entity->getAllAnimationStates();		
			if(animationStateSet && animationStateSet->hasAnimationState("Walk"))
			{
				Ogre::AnimationState* walkAnimationState = animationStateSet->getAnimationState("Walk");
				walkAnimationState->addTime(timeElapsedInSeconds);
			}
		}

		float distance = mCameraSpeed * timeElapsedInSeconds;

		if(mKeyStates[Qt::Key_W] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() + mCamera->getDirection() * distance);
		}
		if(mKeyStates[Qt::Key_S] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() - mCamera->getDirection() * distance);
		}
		if(mKeyStates[Qt::Key_A] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() - mCamera->getRight() * distance);
		}
		if(mKeyStates[Qt::Key_D] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() + mCamera->getRight() * distance);
		}

		if(!mIsFirstFrame)
		{
			QPoint mouseDelta = mCurrentMousePos - mLastFrameMousePos;
			mCamera->yaw(Ogre::Radian(-mouseDelta.x() * timeElapsedInSeconds));
			mCamera->pitch(Ogre::Radian(-mouseDelta.y() * timeElapsedInSeconds));

			int wheelDelta = mCurrentWheelPos - mLastFrameWheelPos;
			Ogre::Radian fov = mCamera->getFOVy();
			fov += Ogre::Radian(-wheelDelta * 0.001);
			fov = (std::min)(fov, Ogre::Radian(2.0f));
			fov = (std::max)(fov, Ogre::Radian(0.5f));
			mCamera->setFOVy(fov);
		}
		mLastFrameMousePos = mCurrentMousePos;
		mLastFrameWheelPos = mCurrentWheelPos;

		mIsFirstFrame = false;
	}

	void DemoGameLogic::shutdown(void)
	{
		mSceneManager->clearScene();
		Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
	}

	void DemoGameLogic::onKeyPress(QKeyEvent* event)
	{
		mKeyStates[event->key()] = KS_PRESSED;

		if(event->key() == Qt::Key_Escape)
		{
			//qApp->centerWidget(mMainMenu, qApp->mMainWindow);
			mMainMenu->exec();
		}
	}

	void DemoGameLogic::onKeyRelease(QKeyEvent* event)
	{
		mKeyStates[event->key()] = KS_RELEASED;
	}

	void DemoGameLogic::onMousePress(QMouseEvent* event)
	{
		mCurrentMousePos = event->pos();
		mLastFrameMousePos = mCurrentMousePos;
	}

	void DemoGameLogic::onMouseMove(QMouseEvent* event)
	{
		mCurrentMousePos = event->pos();
	}

	void DemoGameLogic::onWheel(QWheelEvent* event)
	{
		mCurrentWheelPos += event->delta();
	}

	Log* DemoGameLogic::demoLog(void)
	{
		return mDemoLog;
	}

	void DemoGameLogic::loadScene(QString filename)
	{
		//The QtOgre DotScene loading code will clear the existing scene except for cameras, as these
		//could be used by existing viewports. Therefore we clear and viewports and cameras before
		//calling the loading code.
		mApplication->ogreRenderWindow()->removeAllViewports();
		mSceneManager->destroyAllCameras();

		//Now load the scene.
		DotSceneHandler handler(mSceneManager);
		QXmlSimpleReader reader;
		reader.setContentHandler(&handler);
		reader.setErrorHandler(&handler);

		QFile file(filename);
		file.open(QFile::ReadOnly | QFile::Text);
		QXmlInputSource xmlInputSource(&file);
		reader.parse(xmlInputSource);

		//Now create a viewport, using the first camera in the scene.
		mCamera = mSceneManager->getCameraIterator().peekNextValue();

		//mCamera->setPosition(0, 0, 20);
		//mCamera->lookAt(0, 0, 0);
		mCamera->setNearClipDistance(1.0);
		mCamera->setFarClipDistance(1000.0);
		//mCamera->setFOVy(Ogre::Radian(1.0f));

		Ogre::Viewport* viewport = mApplication->ogreRenderWindow()->addViewport(mCamera);
		viewport->setBackgroundColour(Ogre::ColourValue::Black);
	}
}