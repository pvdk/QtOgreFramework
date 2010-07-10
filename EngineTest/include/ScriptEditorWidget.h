#ifndef ENGINETEST_SCRIPTEDITORWIDGET_H_
#define ENGINETEST_SCRIPTEDITORWIDGET_H_

#include "ui_ScriptEditorWidget.h"

class ScriptEditorWidget : public QWidget, private Ui::ScriptEditorWidget
{
	Q_OBJECT

public:
	ScriptEditorWidget(QWidget *parent = 0);
};

#endif /*ENGINETEST_SCRIPTEDITORWIDGET_H_*/
