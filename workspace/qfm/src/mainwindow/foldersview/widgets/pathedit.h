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
#ifndef PATHEDIT_H_
#define PATHEDIT_H_

#include <QtGui/QLineEdit>
#include <tools/events/qt/eventhandler.h>


class PathEdit : public QLineEdit
{
	Q_OBJECT

public:
	PathEdit(::Tools::Events::EventHandler *eventHandler, QWidget *parent = 0);

	void startEdit();
	void cancelEdit();

protected:
	virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

private:
    ::Tools::Events::EventHandler *m_eventHandler;
    QString m_backupText;
};

#endif /* PATHEDIT_H_ */
