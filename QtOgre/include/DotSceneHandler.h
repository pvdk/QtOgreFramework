#ifndef __DotSceneHandler_H__
#define __DotSceneHandler_H__

#include <OgrePrerequisites.h>

#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>

///NOTE: Any cameras found in the scene file are added to the scene manager, but any existing
///Cameras are not removed. This is because they me be in use by viewports.
///See http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html#a5b2047b5740b691b0e636d57f2dba7e
class DotSceneHandler : public QXmlDefaultHandler
{
public:
	DotSceneHandler(Ogre::SceneManager* sceneManager);

	bool DotSceneHandler::startElement(const QString &, const QString &, const QString& qName, const QXmlAttributes &attributes);
	bool DotSceneHandler::endElement(const QString &, const QString &, const QString& qName);

private:
	Ogre::SceneManager* mSceneManager;
	Ogre::SceneNode* mCurrentNode;
};

#endif //__DotSceneHandler_H__
