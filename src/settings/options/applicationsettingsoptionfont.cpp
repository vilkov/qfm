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
#include "applicationsettingsoptionfont.h"
#include "../../application.h"

#include <QtGui/QFontDialog>


SETTINGS_NS_BEGIN

AppllicationOptionFont::AppllicationOptionFont(const QString &label, const QString &id, Option *parent, const QFont &defaultValue) :
	VisibleOptioinFont(label, id, parent, defaultValue)
{}

bool AppllicationOptionFont::chooseFont(QFont &font) const
{
	bool ok;
	font = QFontDialog::getFont(&ok, font, Application::mainWindow());
	return ok;
}

void AppllicationOptionFont::applyFont(const QFont &font)
{
	Application::setFont(font);
}

SETTINGS_NS_END
