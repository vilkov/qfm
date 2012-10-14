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
#ifndef SETTINGSVISIBLEOPTIONFONT_H_
#define SETTINGSVISIBLEOPTIONFONT_H_

#include "settingsivisibleoption.h"
#include "../value/settingsoptionfont.h"
#include "../../../events/qt/imp/mouseeventhandler.h"


class QLabel;


SETTINGS_NS_BEGIN

class VisibleOptioinFont : public OptionFont, public IVisibleOption
{
public:
	VisibleOptioinFont(const QString &label, const QString &id, Option *parent, const QFont &defaultValue);

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

protected:
	virtual bool chooseFont(QFont &font) const = 0;
	virtual void applyFont(const QFont &font) = 0;

private:
	typedef Events::MouseReleaseEventHandler<
				Events::EventHandlerBase<
					VisibleOptioinFont
				>
			> EventHandler;
	void chooseFontEvent();

private:
	EventHandler m_eventHandler;
	QString m_label;
	QFont m_currentFont;
	QLabel *m_editor;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONFONT_H_ */
