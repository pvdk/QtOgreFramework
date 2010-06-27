#include <Application.h>
#include "EngineTestGameLogic.h"

#include <QPushButton>
#include <QIcon>

using namespace QtOgre;

int main(int argc, char *argv[])
{
	Application app(argc, argv, new EngineTestGameLogic);
	return app.exec(QtOgre::DisplaySettingsDialog);
}