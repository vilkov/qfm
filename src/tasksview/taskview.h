/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
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
