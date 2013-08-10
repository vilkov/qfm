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
#ifndef SETTINGSOPTION_H_
#define SETTINGSOPTION_H_

#include <QtCore/QString>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
#include "../settings_ns.h"


SETTINGS_NS_BEGIN

class Option
{
public:
	typedef QString Id;

public:
	Option(const Id &id, Option *parent = 0) :
	    m_id(id),
		m_parent(parent)
	{}
	virtual ~Option();

    const Id &id() const { return m_id; }
	Option *parent() const { return m_parent; }

    template <typename R> inline
    R *as() { return static_cast<R *>(this); }

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(this); }

protected:
    friend class Scope;
    friend class Container;
    friend class OptionList;
    virtual void save(QXmlStreamWriter &stream) const = 0;
    virtual void load(QXmlStreamReader &stream) = 0;
    virtual void loadDefault() = 0;

protected:
    inline bool readNextStartElement(QXmlStreamReader &stream) const
    {
    	for (QXmlStreamReader::TokenType token = stream.readNext(); token != QXmlStreamReader::StartElement; token = stream.readNext())
    		if ((token == QXmlStreamReader::EndElement && stream.name() == id()) || token == QXmlStreamReader::NoToken || token == QXmlStreamReader::Invalid)
    			return false;

    	return true;
    }

private:
    Id m_id;
	Option *m_parent;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTION_H_ */
