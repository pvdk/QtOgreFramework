#include "Application.h"

#include "EventHandlingOgreWidget.h"
#include "FPSDialog.h"
#include "GameLogic.h"
#include "GraphicsSettingsWidget.h"
#include "LogManager.h"
#include "SettingsDialog.h"

#include <OgreRenderSystem.h>
#include <OgreRoot.h>

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

//Q_INIT_RESOURCE cannot be called from within a namespace, so we provide
//this function. See the Q_INIT_RESOURCE documentation for an explanation.
inline void initQtResources() { Q_INIT_RESOURCE(resources); }

namespace QtOgre
{
	void qtMessageHandler(QtMsgType type, const char* msg)
     {
         switch (type)
		 {
         case QtDebugMsg:
             qApp->_systemLog()->logMessage(msg, LL_DEBUG);
             break;
         case QtWarningMsg:
             qApp->_systemLog()->logMessage(msg, LL_WARNING);
             break;
         case QtCriticalMsg:
             qApp->_systemLog()->logMessage(msg, LL_ERROR);
             break;
         case QtFatalMsg:
			 //We don't override this one as we are dying and wouldn't see it.
             fprintf(stderr, "Fatal: %s\n", msg);
             abort();
         }
     }

	Application::Application(int& argc, char** argv, GameLogic* gameLogic, OgreConfigFiles configFilesToWarnAbout)
	:QApplication(argc, argv)
	,mOpenGLRenderSystem(0)
	,mDirect3D9RenderSystem(0)
	,mFrameCounter(0)
	,mAutoUpdateEnabled(true)
	,mIsInitialised(false)
	,mConfigFilesToWarnAbout(configFilesToWarnAbout)
	{
		mGameLogic = gameLogic;
		if(mGameLogic != 0)
		{
			mGameLogic->mApplication = this;
		}

		//Initialise the resources.
		initQtResources();

		mAutoUpdateTimer = new QTimer;
		QObject::connect(mAutoUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
		//On the test system, a value of one here gives a high frame rate and still allows
		//event prcessing to take place. A value of 0 doubles the frame rate but the mouse
		//becomes jumpy. This property is configerable via setAutoUpdateInterval().
		mAutoUpdateTimer->setInterval(1);

		//Load the settings file. If it doesn't exist it is created.
		mSettings = new QSettings("settings.ini", QSettings::IniFormat);

		mOgreWidget = new EventHandlingOgreWidget(0, 0);

		//Logging should be initialised ASAP, and before Ogre::Root is created.
		initialiseLogging();

		//Create the root and load render system plugins. We do that here so that we know
		//what render systems are available by the time we show the settings dialog.
		//Note that the render system is not initialised until the user selects one.
		if(QFile::exists("plugins.cfg"))
		{
			if(mConfigFilesToWarnAbout & PluginsCfg)
			{
				warnAboutDeprecatedConfigFile("plugins.cfg");
			}
			mRoot = new Ogre::Root();			
		}
		else
		{
			showErrorMessageBox("Loading plugins from settings.ini not implemented yet.");
		}

		mOpenGLRenderSystem = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
		mDirect3D9RenderSystem = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
		if(!(mOpenGLRenderSystem || mDirect3D9RenderSystem))
		{
			qCritical("No rendering subsystems found");
			showErrorMessageBox("No rendering subsystems found. Please ensure that your 'plugins.cfg' is correct and can be found by the executable.");
		}

		mSettingsDialog = new SettingsDialog(mSettings, mOgreWidget);
		mGraphicsSettingsWidget = new GraphicsSettingsWidget;
		mSettingsDialog->addSettingsWidget("Graphics", mGraphicsSettingsWidget);
	}

	Application::~Application()
	{
		if(mOgreWidget)
		{
			delete mOgreWidget;
			mOgreWidget = 0;
		}
		if (mRoot)
		{
            delete mRoot;
			mRoot = 0;
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return whether the OpenGL render system is available
	////////////////////////////////////////////////////////////////////////////////
	bool Application::isOpenGLAvailable(void) const
	{
		return mOpenGLRenderSystem != 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return whether the Direct3D9 render system is available.
	////////////////////////////////////////////////////////////////////////////////
	bool Application::isDirect3D9Available(void) const
	{
		return mDirect3D9RenderSystem != 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return the number of frames rendered since the application started.
	////////////////////////////////////////////////////////////////////////////////
	unsigned int Application::frameCount(void) const
	{
		return mFrameCounter;
	}

	////////////////////////////////////////////////////////////////////////////////
	///This function is an implementation detail, and should not really be exposed.
	///It return the log which the QtOgre framework uses for its messages, whereas
	///users are expected to instead create their own log with createLog(). The reason
	///it is exposed is that the Qt debugging system (qtMessageHandler()) also redirects
	///to this log, and that cannot be made a member function.
	/// \return the log used by the QtOgre framework.
	////////////////////////////////////////////////////////////////////////////////
	Log* Application::_systemLog(void) const
	{
		return mSystemLog;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return a pointer to the applications main window.
	////////////////////////////////////////////////////////////////////////////////
	QWidget* Application::mainWidget(void) const
	{
		return mOgreWidget;
	}

	void Application::applySettings(void)
	{
		//Eventually Application settings might be applied here.
		//Until then, we just pass the settings on the the MainWindow and GameLogic
		if(!mOgreWidget->applySettings(mSettingsDialog->mSettings))
		{
			showWarningMessageBox("Unable to apply desired settings to the window.\nPlease consult the system log for details");
		}

		/*if(mGameLogic != 0)
		{
			mGameLogic->applySettings(mSettings);
		}*/
	}

	void Application::initialise(void)
	{

		mOgreWidget->show();
		mOgreWidget->resize(800,600);
		centerWidget(mOgreWidget);

		centerWidget(mLogManager, mOgreWidget);

		mLogManager->setForceProcessEvents(true);
		initialiseOgre();
		
		
		mLogManager->setForceProcessEvents(false);

		//mLogManager->hide();




		mOgreWidget->setEventHandler(mGameLogic);

		//This is a bit of a hack, necessary because we want to use the settings dialog in two different
		//ways. The first time it is shown (by Application::exec()) no slot is connected - the Accepted
		//event is handled explicitly because the system is not initialised at that point. But now (and
		//when the dialog is shown in future) we are ready for it, so we connect the accepted() signal.
		//We also call accept, to do the initial setup. See also Application::exec().
		connect(mSettingsDialog, SIGNAL(accepted()), this, SLOT(applySettings()));
		mSettingsDialog->accept();

		Ogre::NameValuePairList ogreWindowParams;
		//ogreWindowParams["FSAA"] = "8";
		mOgreWidget->initialise(&ogreWindowParams);

		mFPSDialog = new FPSDialog(mOgreWidget, Qt::FramelessWindowHint);
		mFPSDialog->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());
		mFPSDialog->move(mainWidget()->geometry().topLeft() + QPoint(10,10));

		mLogManager->move(mainWidget()->geometry().left() + 10, mainWidget()->geometry().top() + mainWidget()->geometry().height() - mLogManager->frameGeometry().height() - 10);

		mGameLogic->initialise();

		if(mAutoUpdateEnabled)
		{
			mAutoUpdateTimer->start();
		}

		mIsInitialised = true;
	}

	void Application::update(void)
	{
		mGameLogic->update();
		mOgreWidget->update();
		++mFrameCounter;
	}

	void Application::shutdown(void)
	{
		mAutoUpdateTimer->stop();
		mGameLogic->shutdown();
		mInternalOgreLog->removeListener(this);
		this->exit(0);
	}

	void Application::initialiseLogging(void)
	{
		//Initialise our logging system
		mLogManager = new LogManager(mOgreWidget);
		mLogManager->resize(550, 400);
		mLogManager->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());

		//Redirect Qt's logging system to our own
		mSystemLog = mLogManager->createLog("System");
		qInstallMsgHandler(&qtMessageHandler);
		qDebug("Debug test");
		qWarning("Warning test");
		qCritical("Critical test");

		//Redirect Ogre's logging system to our own
		//This has to all be done before creating the Root object.
		mOgreLog = mLogManager->createLog("Ogre");
		mLogManager->setVisibleLog(mOgreLog);
		mInternalOgreLogManager = new Ogre::LogManager();
		mInternalOgreLog = mInternalOgreLogManager->createLog("Ogre.log", false, true, true);
		mInternalOgreLog->addListener(this);
	}

	void Application::initialiseOgre(void)
	{
		QString renderSystem = mSettingsDialog->mSettings->value("Graphics/RenderSystem").toString();
		if(renderSystem.compare("OpenGL Rendering Subsystem") == 0)
		{
			mActiveRenderSystem = mOpenGLRenderSystem;
		}
		if(renderSystem.compare("Direct3D9 Rendering Subsystem") == 0)
		{
			mActiveRenderSystem = mDirect3D9RenderSystem;
		}

		Ogre::Root::getSingletonPtr()->setRenderSystem(mActiveRenderSystem);

		Ogre::Root::getSingletonPtr()->initialise(false);
	}

	void Application::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName)
	{
		//Convert message to Qt's string type.
		QString messageAsQString = QString::fromStdString(message);

		//Map Ogre's LogMessageLevels to our LogLevels
		switch(lml)
		{
		case Ogre::LML_TRIVIAL:
			mOgreLog->logMessage(messageAsQString, LL_DEBUG);
			break;
		case Ogre::LML_NORMAL:
			mOgreLog->logMessage(messageAsQString, LL_INFO);
			break;
		default: //Should be Ogre::LML_CRITICAL
			mOgreLog->logMessage(messageAsQString, LL_ERROR);
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param displaySettingsDialog should the settings dialog be displayed
	/// \return the application return code
	////////////////////////////////////////////////////////////////////////////////
	int Application::exec(SettingsDialogMode eDialogMode)
	{
		//If we don't show the setting dialog, or we do show it and it is accepted, then proceed.
		if((eDialogMode == SuppressSettingsDialog) || (qApp->showSettingsDialog() == QDialog::Accepted))
		{
			qApp->initialise();
			return QApplication::exec();
		}
		//Otherwise the user cancelled so exit.
		else
		{
			return 0;
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	/// Centers a widget inside its parent, or inside the desktop if no
	/// parent is provided. This requires the widget to be free to move.
	/// If not (e.g. its in a layout) then the behaviour is undefined
	/// \param widgetToCenter the widget to centre
	/// \param parent the parent of the widget
	////////////////////////////////////////////////////////////////////////////////	
	void Application::centerWidget(QWidget* widgetToCenter, QWidget* parent)
	{
		QRect parentGeometry;
		if(parent != 0)
		{
			parentGeometry = parent->frameGeometry();
		}
		else
		{
			parentGeometry = desktop()->availableGeometry();
		}

		int xOffset = (parentGeometry.width() - widgetToCenter->frameGeometry().width()) / 2;
		int yOffset = (parentGeometry.height() - widgetToCenter->frameGeometry().height()) / 2;
		widgetToCenter->move(parentGeometry.x() + xOffset, parentGeometry.y() + yOffset);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param text the text to display
	////////////////////////////////////////////////////////////////////////////////
	void Application::showInfoMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Information");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-information.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param text the text to display
	////////////////////////////////////////////////////////////////////////////////
	void Application::showWarningMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Warning");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-warning.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param text the text to display
	////////////////////////////////////////////////////////////////////////////////
	void Application::showErrorMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-error.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	void Application::hideSettingsDialog(void)
	{
		mSettingsDialog->reject();
	}

	int Application::showSettingsDialog(void)
	{
		return mSettingsDialog->exec();
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param name the name of the log
	/// \return a pointer to the log
	////////////////////////////////////////////////////////////////////////////////
	Log* Application::createLog(const QString& name)
	{
		return mLogManager->createLog(name);
	}

	void Application::hideLogManager(void)
	{
		mLogManager->setVisible(false);
	}

	void Application::showLogManager(void)
	{
		mLogManager->setVisible(true);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return a pointer to the Ogre RenderWindow
	//////////////////////////////////////////////////////////////////////////////// 
	Ogre::RenderWindow* Application::ogreRenderWindow(void) const
	{
		return mOgreWidget->getOgreRenderWindow();
	}

	////////////////////////////////////////////////////////////////////////////////
	// \return a pointer to the application settings
	////////////////////////////////////////////////////////////////////////////////
	QSettings* Application::settings(void) const
	{
		return mSettings;
	}

	/**
	* Sets the period between sucessive updates.
	* \param intervalInMilliseconds the period between sucessive updates
	*/
	void Application::setAutoUpdateInterval(int intervalInMilliseconds)
	{
		mAutoUpdateTimer->setInterval(intervalInMilliseconds);
	}

	void Application::hideFPSCounter(void)
	{
		mFPSDialog->setVisible(false);
	}

	void Application::showFPSCounter(void)
	{
		mFPSDialog->setVisible(true);
	}

	void Application::setAutoUpdateEnabled(bool autoUpdateEnabled)
	{
		mAutoUpdateEnabled = autoUpdateEnabled;

		//Only call start if the app is initialised, otherwise
		//the update() function might be using null pointers.
		if(mAutoUpdateEnabled && mIsInitialised)
		{
			mAutoUpdateTimer->start();
		}
		else
		{
			mAutoUpdateTimer->stop();
		}
	}

	void Application::warnAboutDeprecatedConfigFile(const Ogre::String& filename)
	{
		std::string message =
			"The file \'" + filename + "\' has been found in the applications working directory." +
			"\n\n" +
			"Although this file is usually used by Ogre and/or the ExampleApplication framework, the QtOgre framework has deprecated " +
			"this functionality in favour of using the QSettings functioanlity provided by Qt. It is recommended you use this instead." +
			"\n\n" +
			"In the mean time, the framework should continue to function correctly using the \'" + filename + "\' file you supplied" +
			"\n\n" +
			"You can suppress this message by passing the appropriate flags to the Application constructor. Please consult the documentation";

		showWarningMessageBox(QString::fromStdString(message));
	}
}
