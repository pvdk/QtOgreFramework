#ifndef __DotSceneHandler_H__
#define __DotSceneHandler_H__

#include <OgrePrerequisites.h>

#include <QPair>
#include <QStack>
#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>

///NOTE: Any cameras found in the scene file are added to the scene manager, but any existing
///Cameras are not removed. This is because they me be in use by viewports.
///See http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html#a5b2047b5740b691b0e636d57f2dba7e
class DotSceneHandler : public QXmlDefaultHandler
{
public:
	DotSceneHandler(Ogre::SceneManager* sceneManager);

	Ogre::Camera* handleCamera(const QXmlAttributes &attributes);
	Ogre::Entity* handleEntity(const QXmlAttributes &attributes);
	Ogre::SceneNode* handleNode(const QXmlAttributes &attributes);
	Ogre::SceneNode* handleNodes(const QXmlAttributes &attributes);
	void* handlePosition(const QXmlAttributes &attributes);
	void* handleRotation(const QXmlAttributes &attributes);
	void* handleScale(const QXmlAttributes &attributes);
	void* handleScene(const QXmlAttributes &attributes);

	bool DotSceneHandler::startElement(const QString &, const QString &, const QString& qName, const QXmlAttributes &attributes);
	bool DotSceneHandler::endElement(const QString &, const QString &, const QString& qName);

private:
	Ogre::SceneManager* mSceneManager;
	QStack< QPair< QString, void* > > mParentOgreObjects;
};

#endif //__DotSceneHandler_H__
