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
#include "idm_staticvaluelistdialog.h"


StaticValueListDialog::StaticValueListDialog(const IdmContainer &container, const EntityValueReader &reader, QWidget *parent) :
    NestedPlainDialog(parent),
    m_handler(this),
    m_widget(&m_handler, container, reader, this)
{
//    const QRect &geometry = m_widget.entity()->listGeometry();
//
//    if (geometry.isValid())
//        setGeometry(geometry);

    setWindowTitle(tr("Values of \"%1\"").arg(toUnicode(m_widget.entity().name())));

    m_handler.registerMouseDoubleClickEventHandler(&StaticValueListDialog::accept);
    m_handler.registerShortcut(Qt::CTRL, Qt::Key_F, &StaticValueListDialog::setFocusToFilter);
    m_widget.setViewToolTip(tr("CTRL+F - activate filter field"));

    setCentralWidget(&m_widget);
}

StaticValueListDialog::~StaticValueListDialog()
{
//    if (m_widget.entity()->listGeometry() != geometry())
//        m_widget.container().updateListGeometry(m_widget.entity(), geometry());
}

EntityValue StaticValueListDialog::takeValue()
{
    return m_widget.takeValue();
}

void StaticValueListDialog::accept()
{
    if (currentIndex().isValid())
        QDialog::accept();
    else
        warning(windowTitle(), tr("You must select a value."));
}

QModelIndex StaticValueListDialog::currentIndex() const
{
    return m_widget.currentIndex();
}

void StaticValueListDialog::setFocusToFilter()
{
    m_widget.setFocusToFilter();
}
