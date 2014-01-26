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
#ifndef IDM_CHOOSEENTITYDIALOG_H_
#define IDM_CHOOSEENTITYDIALOG_H_

#include <QtCore/QList>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../containeres/idm_container.h"


using namespace ::VFS::Plugins::Idm;

class ChooseEntityDialog : public QDialog
{
    Q_OBJECT

public:
    const Entity &value() const { return (*m_container.entities().find(m_comboBox.itemData(m_comboBox.currentIndex()).toLongLong())).second; }
    static Entity chooseFile(const IdmContainer &container, QWidget *parent = 0);
    static Entity chooseProperty(const IdmContainer &container, const Entity &entity, ::EFC::String &propertyName, QWidget *parent = 0);

protected:
    ChooseEntityDialog(const QString &title, const IdmContainer &container, const LiquidDb::Storage::Entities &entities, QWidget *parent = 0);

protected:
    const IdmContainer &m_container;
    QVBoxLayout m_verticatLayout;

private:
    QLabel m_label;
    QComboBox m_comboBox;
    QDialogButtonBox m_buttonBox;
    QHBoxLayout m_horizontalLayout;
};

#endif /* IDM_CHOOSEENTITYDIALOG_H_ */
