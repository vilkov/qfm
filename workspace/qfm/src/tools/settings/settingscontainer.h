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
#ifndef SETTINGSCONTAINER_H_
#define SETTINGSCONTAINER_H_

#include <QtCore/QList>
#include "options/settingslist.h"


SETTINGS_NS_BEGIN

class Container : public List
{
public:
	Container(const QString &storage);

	void save() const;
    void load();

protected:
	void manage(Option *option);

protected:
	virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
	static QString storageLocation(const QString &applicationFolder);

private:
	QString m_storage;
};

SETTINGS_NS_END

#endif /* SETTINGSCONTAINER_H_ */
