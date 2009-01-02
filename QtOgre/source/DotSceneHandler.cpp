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

	void* ogreObject = 0;
	
	if(qName == "camera")
	{
		ogreObject = handleCamera(attributes);
	}
	if(qName == "entity")
	{
		ogreObject = handleEntity(attributes);
	}
	if(qName == "node")
	{
		ogreObject = handleNode(attributes);
	}
	if(qName == "nodes")
	{
		ogreObject = handleNodes(attributes);
	}
	if(qName == "position")
	{
		ogreObject = handlePosition(attributes);
	}
	if(qName == "rotation")
	{
		ogreObject = handleRotation(attributes);
	}
	if(qName == "scale")
	{
		ogreObject = handleScale(attributes);
	}
	if(qName == "scene")
	{
		ogreObject = handleScene(attributes);
	}

	if(ogreObject != 0)
	{		
		QPair< QString, void* > pair(qName, ogreObject);
		mParentOgreObjects.push(pair);
	}

	return true;
}

bool DotSceneHandler::endElement(const QString & /* namespaceURI */,
								 const QString & /* localName */,
								 const QString &qName)
{
	qDebug((QString("Ending Element ") + qName).toStdString().c_str());

	if(qName == "camera")
	{
		mParentOgreObjects.pop();
	}
	if(qName == "entity")
	{
		mParentOgreObjects.pop();
	}
	if(qName == "node")
	{
		mParentOgreObjects.pop();
	}

	return true;
}

Ogre::Camera* DotSceneHandler::handleCamera(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentOgreObjects.top();
	if((topPair.first == "node") || (topPair.first == "nodes"))
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		Ogre::Camera* camera = mSceneManager->createCamera(attributes.value("name").toStdString());
		camera->setFOVy(Ogre::Radian(attributes.value("fov").toFloat()));
		node->attachObject(camera);
		return camera;
	}

	return 0;
}

Ogre::Entity* DotSceneHandler::handleEntity(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentOgreObjects.top();
	if((topPair.first == "node") || (topPair.first == "nodes"))
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		Ogre::Entity* entity = mSceneManager->createEntity(attributes.value("name").toStdString(), attributes.value("meshFile").toStdString());
		node->attachObject(entity);

		return entity;
	}
	return 0;
}

Ogre::SceneNode* DotSceneHandler::handleNode(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentOgreObjects.top();
	if((topPair.first == "node") || (topPair.first == "nodes"))
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		return node->createChildSceneNode(attributes.value("name").toStdString());
	}
	return 0;
}

Ogre::SceneNode* DotSceneHandler::handleNodes(const QXmlAttributes &attributes)
{
	return mSceneManager->getRootSceneNode();
}

void* DotSceneHandler::handlePosition(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentOgreObjects.top();
	if(topPair.first == "camera")
	{
		Ogre::Camera* camera = static_cast<Ogre::Camera*>(topPair.second);
		camera->setPosition(attributes.value("x").toFloat(), attributes.value("y").toFloat(), attributes.value("z").toFloat());
	}
	if(topPair.first == "node")
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		node->setPosition(attributes.value("x").toFloat(), attributes.value("y").toFloat(), attributes.value("z").toFloat());
	}

	return 0;
}

void* DotSceneHandler::handleRotation(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentOgreObjects.top();
	if(topPair.first == "node")
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		node->setOrientation(attributes.value("qw").toFloat(), attributes.value("qx").toFloat(), attributes.value("qy").toFloat(), attributes.value("qz").toFloat());
	}
	return 0;
}

void* DotSceneHandler::handleScale(const QXmlAttributes &attributes)
{
	QPair < QString, void* >topPair = mParentOgreObjects.top();
	if(topPair.first == "node")
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		node->setScale(attributes.value("x").toFloat(), attributes.value("y").toFloat(), attributes.value("z").toFloat());
	}
	return 0;
}

void* DotSceneHandler::handleScene(const QXmlAttributes &attributes)
{
	mSceneManager->clearScene();

	return mSceneManager;
}
