#include "EngineTestGameLogic.h"

#include "Application.h"
#include "DotSceneHandler.h"
#include "Scripting/OgreRadianClass.h"
#include "Scripting/OgreVector3Class.h"

#include "LogManager.h"
#include "OgreWidget.h"

#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QSettings>
#include <QStringList>
#include <QMetaObject>

using namespace QtOgre;

EngineTestGameLogic::EngineTestGameLogic(void)
	:GameLogic()
	,m_bRunScript(true)
{
	
}

void EngineTestGameLogic::initialise(void)
{
	mouse = new Mouse(this);
	camera = new Camera(this);

	initScriptEngine();	 

	initScriptEnvironment();

	//debugger.attachTo(scriptEngine);
	//debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
		
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

	mSceneManager->setAmbientLight( Ogre::ColourValue( 0.3f, 0.3f, 0.3f ) );

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

	m_pScriptEditorWidget = new ScriptEditorWidget(qApp->mainWidget());
	m_pScriptEditorWidget->show();

	connect(m_pScriptEditorWidget, SIGNAL(start(void)), this, SLOT(startScriptingEngine(void)));
	connect(m_pScriptEditorWidget, SIGNAL(stop(void)), this, SLOT(stopScriptingEngine(void)));

	mInitialiseScript =
		"print('QtScript Initialisation Begin');"

		"var redLight = new Light();"
		"redLight.position = new QVector3D(100,100,100);"
		"redLight.colour = new QColor(255,0,0);"
		"objectStore.setObject('RedLight', redLight);"

		"var greenLight = new Light();"
		"greenLight.position = new QVector3D(100,100,100);"
		"greenLight.colour = new QColor(0,255,0);"
		"objectStore.setObject('GreenLight', greenLight);"

		"var blueLight = new Light();"
		"blueLight.position = new QVector3D(100,100,100);"
		"blueLight.colour = new QColor(0,0,255);"
		"objectStore.setObject('BlueLight', blueLight);"

		"var robot = new Entity();"
		"robot.meshName = 'robot.mesh';"
		"objectStore.setObject('Robot', robot);"

		"camera.position = new QVector3D(0,0,20);"

		"print('QtScript Initialisation End');";

	QScriptValue result = scriptEngine->evaluate(mInitialiseScript);
		if (scriptEngine->hasUncaughtException())
		{
			int line = scriptEngine->uncaughtExceptionLineNumber();
			qCritical() << "uncaught exception at line" << line << ":" << result.toString();
		}

}

void EngineTestGameLogic::update(void)
{
	mGlobals->setPreviousFrameTime(mGlobals->getCurrentFrameTime());
	mGlobals->setCurrentFrameTime(mTime->elapsed());

	float timeElapsedInSeconds = (mGlobals->getCurrentFrameTime() - mGlobals->getPreviousFrameTime()) / 1000.0f;

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


	if(m_bRunScript)
	{
		QScriptValue result = scriptEngine->evaluate(m_pScriptEditorWidget->getScriptCode());
		if (scriptEngine->hasUncaughtException())
		{
			int line = scriptEngine->uncaughtExceptionLineNumber();
			qCritical() << "uncaught exception at line" << line << ":" << result.toString();
		}
	}

	//mouse->resetDelta();
	mouse->setPreviousPosition(mouse->position());
	mouse->resetWheelDelta();

	mSceneManager->destroyAllLights();
	QHashIterator<QString, QObject*> objectIter(mObjectStore);
	while(objectIter.hasNext())
	{
		objectIter.next();
		QObject* pObj = objectIter.value();

		Light* light = dynamic_cast<Light*>(pObj);
		if(light)
		{
			Ogre::Light* ogreLight = mSceneManager->createLight(objectIter.key().toStdString());
			ogreLight->setType(Ogre::Light::LT_POINT);

			QVector3D pos = light->position();
			ogreLight->setPosition(Ogre::Vector3(pos.x(), pos.y(), pos.z()));

			QColor col = light->getColour();
			ogreLight->setDiffuseColour(col.redF(), col.greenF(), col.blueF());
		}

		Entity* entity = dynamic_cast<Entity*>(pObj);
		if(entity)
		{
			if(mSceneManager->hasEntity(objectIter.key().toStdString()) == false)
			{
				Ogre::Entity* ogreEntity = mSceneManager->createEntity(objectIter.key().toStdString(), entity->meshName().toStdString());
				mSceneManager->getRootSceneNode()->attachObject(ogreEntity);
			}
		}
	}
	
	mCamera->setPosition(Ogre::Vector3(camera->position().x(), camera->position().y(), camera->position().z()));
	//mCamera->setDirection(Ogre::Vector3(camera->direction().x(), camera->direction().y(), camera->direction().z()));

	//QVector3D target = camera->position() - camera->zAxis();	
	//mCamera->lookAt(target.x(), target.y(), target.z());

	mCamera->setOrientation(Ogre::Quaternion(camera->orientation().scalar(), camera->orientation().x(), camera->orientation().y(), camera->orientation().z()));

	mCamera->setFOVy(Ogre::Radian(camera->fieldOfView()));
}

void EngineTestGameLogic::shutdown(void)
{
	mSceneManager->clearScene();
	Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
}

void EngineTestGameLogic::onKeyPress(QKeyEvent* event)
{
	keyboard.press(event->key());
}

void EngineTestGameLogic::onKeyRelease(QKeyEvent* event)
{
	keyboard.release(event->key());
}

void EngineTestGameLogic::onMousePress(QMouseEvent* event)
{
	mouse->press(event->button());
	
	//Update the mouse position as well or we get 'jumps'
	mouse->setPosition(event->pos());
	mouse->setPreviousPosition(mouse->position());
}

void EngineTestGameLogic::onMouseRelease(QMouseEvent* event)
{
	mouse->release(event->button());
}

void EngineTestGameLogic::onMouseMove(QMouseEvent* event)
{
	//mCurrentMousePos = event->pos();
	mouse->setPosition(event->pos());
}

void EngineTestGameLogic::onWheel(QWheelEvent* event)
{
	//mCurrentWheelPos += event->delta();
	mouse->modifyWheelDelta(event->delta());
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
}

void EngineTestGameLogic::initScriptEngine(void)
{
	scriptEngine = new QScriptEngine;

	QStringList extensions;
    extensions << "qt.core"
               << "qt.gui"
               << "qt.xml"
               << "qt.svg"
               << "qt.network"
               << "qt.sql"
               << "qt.opengl"
               << "qt.webkit"
               << "qt.xmlpatterns"
               << "qt.uitools";
    QStringList failExtensions;
    foreach (const QString &ext, extensions)
	{
        QScriptValue ret = scriptEngine->importExtension(ext);
        if (ret.isError())
		{
            failExtensions.append(ext);
		}
    }
    if (!failExtensions.isEmpty())
	{
        if (failExtensions.size() == extensions.size())
		{
            qWarning("Failed to import Qt bindings!\n"
                     "Plugins directory searched: %s/script\n"
                     "Make sure that the bindings have been built, "
                     "and that this executable and the plugins are "
                     "using compatible Qt libraries.", qPrintable(qApp->libraryPaths().join(", ")));
        }
		else
		{
            qWarning("Failed to import some Qt bindings: %s\n"
                     "Plugins directory searched: %s/script\n"
                     "Make sure that the bindings have been built, "
                     "and that this executable and the plugins are "
                     "using compatible Qt libraries.",
                     qPrintable(failExtensions.join(", ")), qPrintable(qApp->libraryPaths().join(", ")));
        }
    }
}

void EngineTestGameLogic::initScriptEnvironment(void)
{
	mGlobals = new Globals(this);

	QScriptValue lightClass = scriptEngine->scriptValueFromQMetaObject<Light>();
	scriptEngine->globalObject().setProperty("Light", lightClass);

	QScriptValue entityClass = scriptEngine->scriptValueFromQMetaObject<Entity>();
	scriptEngine->globalObject().setProperty("Entity", entityClass);

	QScriptValue globalsScriptValue = scriptEngine->newQObject(mGlobals);
	scriptEngine->globalObject().setProperty("globals", globalsScriptValue);

	QScriptValue keyboardScriptValue = scriptEngine->newQObject(&keyboard);
	scriptEngine->globalObject().setProperty("keyboard", keyboardScriptValue);

	QScriptValue mouseScriptValue = scriptEngine->newQObject(mouse);
	scriptEngine->globalObject().setProperty("mouse", mouseScriptValue);

	QScriptValue cameraScriptValue = scriptEngine->newQObject(camera);
	scriptEngine->globalObject().setProperty("camera", cameraScriptValue);

	QScriptValue Qt = scriptEngine->newQMetaObject(&staticQtMetaObject);
	Qt.setProperty("App", scriptEngine->newQObject(qApp));
	scriptEngine->globalObject().setProperty("Qt", Qt);

	QScriptValue objectStoreScriptValue = scriptEngine->newQObject(&mObjectStore);
	scriptEngine->globalObject().setProperty("objectStore", objectStoreScriptValue);

	OgreVector3Class *vecClass = new OgreVector3Class(scriptEngine);
	scriptEngine->globalObject().setProperty("OgreVector3", vecClass->constructor());

	OgreRadianClass *radianClass = new OgreRadianClass(scriptEngine);
	scriptEngine->globalObject().setProperty("OgreRadian", radianClass->constructor());
}

void EngineTestGameLogic::startScriptingEngine(void)
{
	m_bRunScript = true;
}

void EngineTestGameLogic::stopScriptingEngine(void)
{
	m_bRunScript = false;
}