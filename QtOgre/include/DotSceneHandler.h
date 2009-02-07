#ifndef __DotSceneHandler_H__
#define __DotSceneHandler_H__

#include <OgrePrerequisites.h>

#include <QPair>
#include <QStack>
#include <QXmlDefaultHandler>

///NOTE: Any cameras found in the scene file are added to the scene manager, but any existing
///Cameras are not removed. This is because they may be in use by viewports.
///See http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html#a5b2047b5740b691b0e636d57f2dba7e
class DotSceneHandler : public QXmlDefaultHandler
{
public:
	DotSceneHandler(Ogre::SceneManager* sceneManager);

	bool startElement(const QString &, const QString &, const QString& qName, const QXmlAttributes &attributes);
	bool endElement(const QString &, const QString &, const QString& qName);

private:
	Ogre::Camera* handleCamera(const QXmlAttributes &attributes);
	void* handleClipping(const QXmlAttributes &attributes);
	void* handleColourAmbient(const QXmlAttributes &attributes);
	Ogre::Entity* handleEntity(const QXmlAttributes &attributes);
	Ogre::Light* handleLight(const QXmlAttributes &attributes);
	void* handleLookTarget(const QXmlAttributes &attributes);
	Ogre::SceneNode* handleNode(const QXmlAttributes &attributes);
	Ogre::SceneNode* handleNodes(const QXmlAttributes &attributes);
	void* handleNormal(const QXmlAttributes &attributes);
	void* handlePosition(const QXmlAttributes &attributes);
	void* handleRotation(const QXmlAttributes &attributes);
	void* handleScale(const QXmlAttributes &attributes);
	void* handleScene(const QXmlAttributes &attributes);
	Ogre::SceneNode* handleSkyBox(const QXmlAttributes &attributes);

	bool convertWithDefault(const QString& inputString, bool defaultVal);
	double convertWithDefault(const QString& inputString, double defaultVal);
	float convertWithDefault(const QString& inputString, float defaultVal);
	int convertWithDefault(const QString& inputString, int defaultVal);
	QString convertWithDefault(const QString& inputString, const char* defaultVal);

	///The scene manager into which the XML is being loaded
	Ogre::SceneManager* mSceneManager;
	/**
	There are primarily two types of XML nodes we will encounter. Some (like 'entity', 'camera', etc) are
	actual Ogre objects which can exist in the scene. Others (like 'rotation', 'scale', etc) are transformations
	which should be applied to the Ogre object with the 'most local scope'. As we enter and exit XML nodes we
	push and pop the corresponding objects (or just null pointers) onto the stack below. Then, when we encounter
	a transformation, we just apply it to whatever is at the top of the stack.

	The apporach works, but there might be a better and/or more type safe way to do it. Because any kind of Ogre
	object could be on the stack we store them using void pointers and then use the string to cast to the correct
	type at runtime.
	*/
	QStack< QPair< QString, void* > > mParentObjects;
};

#endif //__DotSceneHandler_H__
