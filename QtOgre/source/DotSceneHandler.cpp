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
	//For debugging
	qDebug((QString("Starting Element ") + qName).toStdString().c_str());

	//This will hold a pointer to any ogre object
	//created when processing this XML node.
	void* ogreObject = 0;
	
	//Call the appropriate helper function depending on the XML node type.
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
	if(qName == "skyBox")
	{
		ogreObject = handleSkyBox(attributes);
	}

	//If an Ogre object was created then add it to the stack.
	//Later transformations will be applied to this top object.
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
	//For debugging
	qDebug((QString("Ending Element ") + qName).toStdString().c_str());

	//If the XML node causes an Ogre object to be added to the stack,
	//we remove it to expose the one which was there previously.
	if
	(
		(qName == "camera") ||
		(qName == "entity") ||
		(qName == "node") ||
		(qName == "skyBox")
	)
	{
		mParentOgreObjects.pop();
	}

	return true;
}

Ogre::Camera* DotSceneHandler::handleCamera(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentOgreObjects.top();
	//Camera can be attached to the root node ('nodes'), or one of it's children ('node').
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
	//Entity can be attached to the root node ('nodes'), or one of it's children ('node').
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
	//Node can be attached to the root node ('nodes'), or one of it's children ('node').
	if((topPair.first == "node") || (topPair.first == "nodes"))
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		return node->createChildSceneNode(attributes.value("name").toStdString());
	}
	return 0;
}

Ogre::SceneNode* DotSceneHandler::handleNodes(const QXmlAttributes &attributes)
{
	//We only expect a single 'nodes' element in the XML, and this should correspond to the root.
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

	//We haven't created any Ogre objects, so nothing to return
	return 0;
}

void* DotSceneHandler::handleRotation(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentOgreObjects.top();
	if(topPair.first == "skyBox")
	{
		//Finding a skybox actually adds a SceneNode to the stack.
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		node->setOrientation(attributes.value("qw").toFloat(), attributes.value("qx").toFloat(), attributes.value("qy").toFloat(), attributes.value("qz").toFloat());
	}
	if(topPair.first == "node")
	{
		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(topPair.second);
		node->setOrientation(attributes.value("qw").toFloat(), attributes.value("qx").toFloat(), attributes.value("qy").toFloat(), attributes.value("qz").toFloat());
	}

	//We haven't created any Ogre objects, so nothing to return
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

	//We haven't created any Ogre objects, so nothing to return
	return 0;
}

void* DotSceneHandler::handleScene(const QXmlAttributes &attributes)
{
	//Removes everything *except* cameras
	mSceneManager->clearScene();
	return mSceneManager;
}

Ogre::SceneNode* DotSceneHandler::handleSkyBox(const QXmlAttributes &attributes)
{	
	//Extract the attributes while allowing default parameters
	QString material = convertWithDefault(attributes.value("material"), "BaseWhiteNoLighting");
	double distance = convertWithDefault(attributes.value("distance"), 5000.0);
	bool drawFirst = convertWithDefault(attributes.value("drawFirst"), true);

	//Set up the skybox
	mSceneManager->setSkyBox(true, material.toStdString(), distance, drawFirst);

	//Return the skybox node - we may apply rotations to this later.
	return mSceneManager->getSkyBoxNode();
}

bool DotSceneHandler::convertWithDefault(const QString& inputString, bool defaultVal)
{
	if(inputString.compare("true"))
	{
		return true;
	}
	if(inputString.compare("false"))
	{
		return false;
	}
	else
	{
		return defaultVal;
	}
}

double DotSceneHandler::convertWithDefault(const QString& inputString, double defaultVal)
{
	bool ok = false;
	double retVal = inputString.toDouble(&ok);
	return (ok ? retVal : defaultVal);
}

float DotSceneHandler::convertWithDefault(const QString& inputString, float defaultVal)
{
	bool ok = false;
	float retVal = inputString.toFloat(&ok);
	return (ok ? retVal : defaultVal);
}

int DotSceneHandler::convertWithDefault(const QString& inputString, int defaultVal)
{
	bool ok = false;
	int retVal = inputString.toInt(&ok);
	return (ok ? retVal : defaultVal);
}

QString DotSceneHandler::convertWithDefault(const QString& inputString, const char* defaultVal)
{
	if(inputString.isEmpty() == false)
	{
		return inputString;
	}
	else
	{
		return defaultVal;
	}
}
