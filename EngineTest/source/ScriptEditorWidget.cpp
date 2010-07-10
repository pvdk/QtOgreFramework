#include "ScriptEditorWidget.h"

#include <QFile>
#include <QTextStream>

ScriptEditorWidget::ScriptEditorWidget(QWidget *parent)
:QWidget(parent, Qt::Tool)
{
	setupUi(this);

	connect(m_pPlayButton, SIGNAL(clicked(void)), this, SIGNAL(play(void)));
	connect(m_pStopButton, SIGNAL(clicked(void)), this, SIGNAL(stop(void)));

	/*QString updateScript =		
		"var pos = camera.position;\n"
		"var dir = camera.direction;\n"		
		"var right = camera.right;\n"
		"if(keyboard.isPressed(Qt.Key_W))"
		"{"			
		//"	pos += dir;"
		"	pos.setX(pos.x() + dir.x());"
		"	pos.setY(pos.y() + dir.y());"
		"	pos.setZ(pos.z() + dir.z());"
		"}"
		"if(keyboard.isPressed(Qt.Key_S))"
		"{"			
		"	pos.setX(pos.x() - dir.x());"
		"	pos.setY(pos.y() - dir.y());"
		"	pos.setZ(pos.z() - dir.z());"
		"}"
		"if(keyboard.isPressed(Qt.Key_A))"
		"{"			
		"	pos.setX(pos.x() - right.x());"
		"	pos.setY(pos.y() - right.y());"
		"	pos.setZ(pos.z() - right.z());"
		"}"
		"if(keyboard.isPressed(Qt.Key_D))"
		"{"			
		"	pos.setX(pos.x() + right.x());"
		"	pos.setY(pos.y() + right.y());"
		"	pos.setZ(pos.z() + right.z());"
		"}"		
		"camera.position = pos;"
		"if(mouse.isPressed(Qt.RightButton))"
		"{"
		"	mouseDelta = mouse.computeDelta();"
		"	camera.yaw(new OgreRadian(-mouseDelta.x * 0.01));"
		"}"
		"if(mouse.isPressed(Qt.RightButton))"
		"{"
		"	mouseDelta = mouse.computeDelta();"
		"	camera.pitch(new OgreRadian(-mouseDelta.y * 0.01));"
		"}"
		"wheelDelta = mouse.getWheelDelta();"
		//"print('wheelDelta = ', wheelDelta);"
		"fov = camera.getFOVy();"
		//"print('old fov = ', fov.valueRadians);"
		"tempFOV = new OgreRadian(-wheelDelta * 0.001);"
		"fov.valueRadians = fov.valueRadians + tempFOV.valueRadians;"
		//"print('new fov = ', fov.valueRadians);"
		"camera.setFOVy(fov);"
		;*/

	QFile updateScriptFile("media\\scripts\\update.js");

	if (updateScriptFile.open(QFile::ReadOnly))
	{
		QTextStream stream(&updateScriptFile);
		m_pTextEdit->setPlainText(stream.readAll());
		updateScriptFile.close();
	}
	else
	{
		m_pTextEdit->setPlainText("////Failed to open file");
	}

	//m_pTextEdit->setPlainText(updateScript);
}

/*void ScriptEditorWidget::on_m_pPlayButton_Clicked(void)
{
}*/

QString ScriptEditorWidget::getScriptCode(void)
{
	return m_pTextEdit->toPlainText();
}