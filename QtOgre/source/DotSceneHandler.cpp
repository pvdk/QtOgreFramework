#include "DotSceneHandler.h"

#include <OgreEntity.h>
#include <OgreSceneManager.h>

DotSceneHandler::DotSceneHandler(Ogre::SceneManager* sceneManager)
{
	mSceneManager = sceneManager;
}

bool DotSceneHandler::startElement(const QString & /* namespaceURI */,
								   const QString & /* localName */,
								   const QString &qName,
								   const QXmlAttributes &attributes)
{
	qDebug((QString("Starting Element ") + qName).toStdString().c_str());
	
	if(qName == "camera")
	{
		Ogre::Camera* camera = mSceneManager->createCamera(attributes.value("name").toStdString());
		camera->setFOVy(Ogre::Radian(attributes.value("fov").toFloat()));
		mCurrentNode->attachObject(camera);
	}
	if(qName == "entity")
	{
		Ogre::Entity* entity = mSceneManager->createEntity(attributes.value("name").toStdString(), attributes.value("meshFile").toStdString());
		mCurrentNode->attachObject(entity);
	}
	if(qName == "node")
	{
		mCurrentNode = mCurrentNode->createChildSceneNode(attributes.value("name").toStdString());
	}
	if(qName == "nodes")
	{
		mCurrentNode = mSceneManager->getRootSceneNode();
	}
	if(qName == "position")
	{
		mCurrentNode->setPosition(attributes.value("x").toFloat(), attributes.value("y").toFloat(), attributes.value("z").toFloat());
	}
	if(qName == "rotation")
	{
		mCurrentNode->setOrientation(attributes.value("qw").toFloat(), attributes.value("qx").toFloat(), attributes.value("qy").toFloat(), attributes.value("qz").toFloat());
	}
	if(qName == "scale")
	{
		mCurrentNode->setScale(attributes.value("x").toFloat(), attributes.value("y").toFloat(), attributes.value("z").toFloat());
	}
	if(qName == "scene")
	{
		mSceneManager->clearScene();
	}

	return true;
}

bool DotSceneHandler::endElement(const QString & /* namespaceURI */,
								 const QString & /* localName */,
								 const QString &qName)
{
	qDebug((QString("Ending Element ") + qName).toStdString().c_str());

	if(qName == "node")
	{
		mCurrentNode = mCurrentNode->getParentSceneNode();
	}

	return true;
}