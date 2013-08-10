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
#ifndef IDM_NEWPROPERTYNAME_H_
#define IDM_NEWPROPERTYNAME_H_

#include <QtCore/QList>
#include <tools/widgets/stringdialog/stringdialog.h>
#include "../../storage/entities/idm_entity.h"


using namespace ::VFS::Plugins::Idm;

class NewPropertyNameDialog : public StringDialog
{
    Q_OBJECT

public:
    typedef QList<Entity::Property> Properties;

public:
    NewPropertyNameDialog(const Properties &properties, QWidget *parent = 0);

protected:
    virtual bool check();

private:
    const Properties &m_properties;
};

#endif /* IDM_NEWPROPERTYNAME_H_ */
