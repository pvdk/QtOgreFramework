#include "DemoGameLogic.h"

#include "DotSceneHandler.h"
#include "MainMenu.h"
#include "Scripting/OgreRadianClass.h"
#include "Scripting/OgreVector3Class.h"

#include "LogManager.h"
#include "OgreWidget.h"
#include "StyleSettingsWidget.h"

#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QSettings>

#include <QMetaObject>

namespace QtOgre
{
	QScriptValue toScriptValue(QScriptEngine *engine, const MyStruct &s)
	{
		QScriptValue obj = engine->newObject();
		obj.setProperty("x", s.x);
		obj.setProperty("y", s.y);
		return obj;
	}

	void fromScriptValue(const QScriptValue &obj, MyStruct &s)
	{
		s.x = obj.property("x").toInt32();
		s.y = obj.property("y").toInt32();
	}

	QScriptValue toScriptValueQPoint(QScriptEngine *engine, const QPoint &s)
	{
		QScriptValue obj = engine->newObject();
		obj.setProperty("x", s.x());
		obj.setProperty("y", s.y());
		return obj;
	}

	void fromScriptValueQPoint(const QScriptValue &obj, QPoint &s)
	{
		s.setX(obj.property("x").toInt32());
		s.setY(obj.property("y").toInt32());
	}

	struct QtMetaObject : private QObject
	{
	public:
		static const QMetaObject *get()
		{ return &static_cast<QtMetaObject*>(0)->staticQtMetaObject; }
	};

	DemoGameLogic::DemoGameLogic(void)
		:GameLogic()
	{
	}

	void DemoGameLogic::initialise(void)
	{
		scriptEngine = new QScriptEngine;

		scriptEngine->importExtension("qt.core");
		scriptEngine->importExtension("qt.gui");


		QScriptValue keyboardScriptValue = scriptEngine->newQObject(&keyboard);
		scriptEngine->globalObject().setProperty("keyboard", keyboardScriptValue);

		QScriptValue mouseScriptValue = scriptEngine->newQObject(&mouse);
		scriptEngine->globalObject().setProperty("mouse", mouseScriptValue);

		QScriptValue cameraScriptValue = scriptEngine->newQObject(&cameraWrapper);
		scriptEngine->globalObject().setProperty("camera", cameraScriptValue);

		QScriptValue Qt = scriptEngine->newQMetaObject(QtMetaObject::get());
		Qt.setProperty("App", scriptEngine->newQObject(qApp));
		scriptEngine->globalObject().setProperty("Qt", Qt);

		myStruct.x = 10;
		qScriptRegisterMetaType(scriptEngine, toScriptValue, fromScriptValue);
		QScriptValue myStructValue = scriptEngine->toScriptValue(myStruct);
		scriptEngine->globalObject().setProperty("myStruct", myStructValue);

		qScriptRegisterMetaType(scriptEngine, toScriptValueQPoint, fromScriptValueQPoint);

		OgreVector3Class *vecClass = new OgreVector3Class(scriptEngine);
		scriptEngine->globalObject().setProperty("OgreVector3", vecClass->constructor());

		OgreRadianClass *radianClass = new OgreRadianClass(scriptEngine);
		scriptEngine->globalObject().setProperty("OgreRadian", radianClass->constructor());

		updateScript =
			"vec = new OgreVector3;"
			"vec.x = 0.0;"
			"vec.y = 0.0;"
			"vec.z = 0.0;"
			"if(keyboard.isPressed(Qt.Key_W))"
			"{"			
			"	vec.z = -1.0;"
			"}"
			"if(keyboard.isPressed(Qt.Key_S))"
			"{"			
			"	vec.z = 1.0;"
			"}"
			"if(keyboard.isPressed(Qt.Key_A))"
			"{"			
			"	vec.x = -1.0;"
			"}"
			"if(keyboard.isPressed(Qt.Key_D))"
			"{"			
			"	vec.x = 1.0;"
			"}"
			"camera.moveRelative(vec);"
			"if(mouse.isPressed(Qt.RightButton))"
			"{"
			"	mouseDelta = mouse.computeDelta();"
			"	camera.yaw(new OgreRadian(-mouseDelta.x * 0.01));"
			"}"
			"if(mouse.isPressed(Qt.RightButton))"
			"{"
			"	mouseDelta = mouse.computeDelta();"
			"	camera.pitch(new OgreRadian(-mouseDelta.y * 0.01));"
			"}"
			;



		//qApp->setStyleSheet(qApp->settings()->value("UI/StyleFile").toString());
		
		mDemoLog = mApplication->createLog("Demo");

		mApplication->showLogManager();

		mDemoLog->logMessage("A demonstration debug message", LL_DEBUG);
		mDemoLog->logMessage("A demonstration info message", LL_INFO);
		mDemoLog->logMessage("A demonstration warning message", LL_WARNING);
		mDemoLog->logMessage("A demonstration error message", LL_ERROR);

		//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		// Create the generic scene manager
		mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "GenericSceneManager");

		//Set up scene
		loadScene("media/scenes/test.scene");

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

		mApplication->showFPSCounter();
		
		mStyleSettingsWidget = new StyleSettingsWidget;
		mApplication->addSettingsWidget("Style", mStyleSettingsWidget);

		/*cameraPositionScriptValue = scriptEngine->toScriptValue(mCamera->getPosition());
		cameraDirectionScriptValue = scriptEngine->toScriptValue(mCamera->getDirection());
		cameraRightScriptValue = scriptEngine->toScriptValue(mCamera->getRight());

		scriptEngine->globalObject().setProperty("cameraPosition", cameraPositionScriptValue);
		scriptEngine->globalObject().setProperty("cameraDirection", cameraDirectionScriptValue);
		scriptEngine->globalObject().setProperty("cameraRight", cameraRightScriptValue);*/
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

		/*if(mouse.isPressed(Qt::LeftButton))
		{
			QPoint mouseDelta = mouse.computeDelta();
			mCamera->yaw(Ogre::Radian(-mouseDelta.x() * timeElapsedInSeconds));
			mCamera->pitch(Ogre::Radian(-mouseDelta.y() * timeElapsedInSeconds));

			int wheelDelta = mouse.computeWheelDelta();
			Ogre::Radian fov = mCamera->getFOVy();
			fov += Ogre::Radian(-wheelDelta * 0.001);
			fov = (std::min)(fov, Ogre::Radian(2.0f));
			fov = (std::max)(fov, Ogre::Radian(0.5f));
			mCamera->setFOVy(fov);
		}*/

		mIsFirstFrame = false;

		QScriptValue result = scriptEngine->evaluate(updateScript);
		if (scriptEngine->hasUncaughtException())
		{
			int line = scriptEngine->uncaughtExceptionLineNumber();
			qCritical() << "uncaught exception at line" << line << ":" << result.toString();
		}

		mouse.resetDelta();
		mouse.resetWheelDelta();
	}

	void DemoGameLogic::shutdown(void)
	{
		mSceneManager->clearScene();
		Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
	}

	void DemoGameLogic::onKeyPress(QKeyEvent* event)
	{
		keyboard.press(event->key());

		if(event->key() == Qt::Key_Escape)
		{
			//qApp->centerWidget(mMainMenu, qApp->mMainWindow);
			mMainMenu->exec();
		}
	}

	void DemoGameLogic::onKeyRelease(QKeyEvent* event)
	{
		keyboard.release(event->key());
	}

	void DemoGameLogic::onMousePress(QMouseEvent* event)
	{
		mouse.press(event->button());

		//Update the mouse position as well or we get 'jumps'
		mouse.setPos(event->pos());
		mouse.resetDelta();
	}

	void DemoGameLogic::onMouseRelease(QMouseEvent* event)
	{
		mouse.release(event->button());
	}

	void DemoGameLogic::onMouseMove(QMouseEvent* event)
	{
		//mCurrentMousePos = event->pos();
		mouse.setPos(event->pos());
	}

	void DemoGameLogic::onWheel(QWheelEvent* event)
	{
		//mCurrentWheelPos += event->delta();
		mouse.setWheelPos(mouse.wheelPos() + event->delta());
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

		cameraWrapper.setOgreCamera(mCamera);
	}
}