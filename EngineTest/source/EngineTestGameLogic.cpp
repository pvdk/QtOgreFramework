#include "EngineTestGameLogic.h"

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

using namespace QtOgre;

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

	EngineTestGameLogic::EngineTestGameLogic(void)
		:GameLogic()
	{
	}

	void EngineTestGameLogic::initialise(void)
	{
		scriptEngine = new QScriptEngine;

		QDir dir("C:\\qt\\qt-labs-qtscriptgenerator\\plugins");
		QStringList paths = qApp->libraryPaths();
		paths <<  dir.absolutePath();
		qApp->setLibraryPaths(paths);

		QString temp;
		foreach(temp, paths)
		{
			qCritical() << temp;
		}

		QScriptValue retCore = scriptEngine->importExtension("qt.core");
		QScriptValue retGui = scriptEngine->importExtension("qt.gui");
		if(retCore.isError())
		{
			qCritical() << "Failed to load core plugin";
		}
		if(retGui.isError())
		{
			qCritical() << "Failed to load gui plugin";
		}


		QScriptValue keyboardScriptValue = scriptEngine->newQObject(&keyboard);
		scriptEngine->globalObject().setProperty("keyboard", keyboardScriptValue);

		QScriptValue mouseScriptValue = scriptEngine->newQObject(&mouse);
		scriptEngine->globalObject().setProperty("mouse", mouseScriptValue);

		QScriptValue cameraScriptValue = scriptEngine->newQObject(&cameraWrapper);
		scriptEngine->globalObject().setProperty("camera", cameraScriptValue);

		QScriptValue Qt = scriptEngine->newQMetaObject(QtMetaObject::get());
		Qt.setProperty("App", scriptEngine->newQObject(qApp));
		scriptEngine->globalObject().setProperty("Qt", Qt);

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
			"var vector = new QVector3D();"
			"vector.setX(-1.0);"
			"print('vector = ', vector);"
			"if(keyboard.isPressed(Qt.Key_W))"
			"{"			
			"	vec.z = vector.x();"
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
			"wheelDelta = mouse.getWheelDelta();"
			"print('wheelDelta = ', wheelDelta);"
			"fov = camera.getFOVy();"
			"print('old fov = ', fov.valueRadians);"
			"tempFOV = new OgreRadian(-wheelDelta * 0.001);"
			"fov.valueRadians = fov.valueRadians + tempFOV.valueRadians;"
			"print('new fov = ', fov.valueRadians);"
			"camera.setFOVy(fov);"
			;

		//debugger.attachTo(scriptEngine);
		//debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();




		//qApp->setStyleSheet(qApp->settings()->value("UI/StyleFile").toString());
		
		mDemoLog = mApplication->createLog("EngineTest");

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

	void EngineTestGameLogic::update(void)
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

	void EngineTestGameLogic::shutdown(void)
	{
		mSceneManager->clearScene();
		Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
	}

	void EngineTestGameLogic::onKeyPress(QKeyEvent* event)
	{
		keyboard.press(event->key());

		if(event->key() == Qt::Key_Escape)
		{
			//qApp->centerWidget(mMainMenu, qApp->mMainWindow);
			mMainMenu->exec();
		}
	}

	void EngineTestGameLogic::onKeyRelease(QKeyEvent* event)
	{
		keyboard.release(event->key());
	}

	void EngineTestGameLogic::onMousePress(QMouseEvent* event)
	{
		mouse.press(event->button());

		//Update the mouse position as well or we get 'jumps'
		mouse.setPos(event->pos());
		mouse.resetDelta();
	}

	void EngineTestGameLogic::onMouseRelease(QMouseEvent* event)
	{
		mouse.release(event->button());
	}

	void EngineTestGameLogic::onMouseMove(QMouseEvent* event)
	{
		//mCurrentMousePos = event->pos();
		mouse.setPos(event->pos());
	}

	void EngineTestGameLogic::onWheel(QWheelEvent* event)
	{
		//mCurrentWheelPos += event->delta();
		mouse.modifyWheelDelta(event->delta());
	}

	Log* EngineTestGameLogic::demoLog(void)
	{
		return mDemoLog;
	}

	void EngineTestGameLogic::loadScene(QString filename)
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