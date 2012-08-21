#ifndef TASKVIEW_H_
#define TASKVIEW_H_

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QHBoxLayout>
#include "../tools/events/qt/imp/mouseeventsource.h"
#include "../tools/events/qt/imp/keyboardeventsource.h"
#include "../tools/events/qt/imp/mouseeventhandler.h"
#include "../tools/events/qt/imp/keyboardeventhandler.h"


using namespace ::Tools::Events;

class TaskView : public QWidget
{
public:
	TaskView(QWidget *parent = 0);

private:
	typedef MouseReleaseEventSource<
				KeyboardEventSource<
					EventSourceBase<
						QToolButton
					>
				>
			> ToolButton;
	typedef MouseReleaseEventHandler<
				KeyboardEventHandler<
					EventHandlerBase<
						TaskView
					>
				>
			> ToolButtonEventHandler;

private:
	ToolButtonEventHandler m_eventHandler;
	QHBoxLayout m_layout;
	QLabel m_text;
	ToolButton m_cancel;
};

#endif /* TASKVIEW_H_ */
