#ifndef __DotSceneHandler_H__
#define __DotSceneHandler_H__

#include <OgrePrerequisites.h>

#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>

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
