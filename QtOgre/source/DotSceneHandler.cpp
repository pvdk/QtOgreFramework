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
	if(qName == "clipping")
	{
		ogreObject = handleClipping(attributes);
	}
	if(qName == "entity")
	{
		ogreObject = handleEntity(attributes);
	}
	if(qName == "lookTarget")
	{
		ogreObject = handleLookTarget(attributes);
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

	//Add any created object (or just a null pointer) onto the stack.
	//Later transformations will be applied to this top object.
	QPair< QString, void* > pair(qName, ogreObject);
	mParentObjects.push(pair);

	return true;
}

bool DotSceneHandler::endElement(const QString & /* namespaceURI */,
								 const QString & /* localName */,
								 const QString &qName)
{
	//For debugging
	qDebug((QString("Ending Element ") + qName).toStdString().c_str());

	//Pop the top most object off the stack.
	mParentObjects.pop();

	return true;
}

Ogre::Camera* DotSceneHandler::handleCamera(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentObjects.top();
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

void* DotSceneHandler::handleClipping(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentObjects.top();
	//Camera can be attached to the root node ('nodes'), or one of it's children ('node').
	if(topPair.first == "camera")
	{
		Ogre::Camera* camera = static_cast<Ogre::Camera*>(topPair.second);
		camera->setNearClipDistance(convertWithDefault(attributes.value("near"), 0.1f));
		camera->setFarClipDistance(convertWithDefault(attributes.value("far"), 1000.0f));
	}

	return 0;
}

Ogre::Entity* DotSceneHandler::handleEntity(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentObjects.top();
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

void* DotSceneHandler::handleLookTarget(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentObjects.top();
	//Entity can be attached to the root node ('nodes'), or one of it's children ('node').
	if(topPair.first == "camera")
	{
		//Nothing to actually do here - lookAt() is called later when we find the position node.
	}
	return 0;
}

Ogre::SceneNode* DotSceneHandler::handleNode(const QXmlAttributes &attributes)
{
	QPair< QString, void* > topPair = mParentObjects.top();
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
	QPair< QString, void* > topPair = mParentObjects.top();
	if(topPair.first == "camera")
	{
		Ogre::Camera* camera = static_cast<Ogre::Camera*>(topPair.second);
		camera->setPosition(attributes.value("x").toFloat(), attributes.value("y").toFloat(), attributes.value("z").toFloat());
	}
	if(topPair.first == "lookTarget")
	{
		//If we have a lookTarget node, we want to go up another level to find what the lookTarget is attached to.
		QPair< QString, void* > nextPair = mParentObjects.at(mParentObjects.size() - 2);
		//Check if we are setting the lookTarget of a camera...
		if(nextPair.first == "camera")
		{
			Ogre::Camera* camera = static_cast<Ogre::Camera*>(nextPair.second);
			camera->lookAt(attributes.value("x").toFloat(), attributes.value("y").toFloat(), attributes.value("z").toFloat());
		}
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
	QPair< QString, void* > topPair = mParentObjects.top();
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
	QPair < QString, void* >topPair = mParentObjects.top();
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
