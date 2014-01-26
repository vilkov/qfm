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
#ifndef IDM_CREATEENTITYDIALOG_H_
#define IDM_CREATEENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/idm_entitieslistmodel.h"
#include "model/idm_entitieslistdelegate.h"


using namespace ::VFS::Plugins::Idm;

class CreateEntityDialog : public QDialog
{
    Q_OBJECT

public:
    typedef EntitiesListModel::size_type size_type;

public:
    CreateEntityDialog(const IdmContainer &container, const QString &name = QString(), QWidget *parent = 0);

    Entity::Type type() const { return static_cast<Entity::Type>(m_comboBox.itemData(m_comboBox.currentIndex(), Qt::UserRole).toInt()); }
    ::EFC::String name() const { return fromUnicode(m_lineEdit.text().simplified()).data(); }
    ::EFC::String title() const { return fromUnicode(m_lineEdit2.text().simplified()).data(); }

    const Entity &property(size_type index) const { return m_model.entityAt(index); }
    const ::EFC::String &propertyName(size_type index) const { return m_model.nameAt(index); }
    size_type size() const { return m_model.size(); }

    virtual void accept();

private Q_SLOTS:
    void activated(int index);
    void add();
    void remove();

private:
    void setListEnabled(bool enabled);

private:
    QLabel m_label;
    QLineEdit m_lineEdit;
    QLabel m_label2;
    QLineEdit m_lineEdit2;
    QLabel m_label3;
    QComboBox m_comboBox;
    QTreeView m_view;
    QPushButton m_addEntity;
    QPushButton m_removeEntity;
    QGridLayout m_gridLayout;
    QGridLayout m_gridLayout2;
    QHBoxLayout m_horizontalLayout;
    QDialogButtonBox m_buttonBox;
    EntitiesListModel m_model;
    EntitiesListDelegate m_delegate;
};

#endif /* IDM_CREATEENTITYDIALOG_H_ */
