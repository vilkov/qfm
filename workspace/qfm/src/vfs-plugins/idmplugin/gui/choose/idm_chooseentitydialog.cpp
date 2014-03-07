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
#include "idm_chooseentitydialog.h"

#include <vfs/model/vfs_proxymodel.h>

#include <QtCore/QSet>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>


template <>
class qLess<Entity>
{
public:
    typedef Entity value_type;

public:
    inline bool operator()(const value_type &t1, const value_type &t2) const
    {
        return ::VFS::ProxyModel::compareFileNames(t1.name().c_str(), t2.name().c_str());
    }
};


class ChoosePropertyDialog : public ChooseEntityDialog
{
public:
    ChoosePropertyDialog(const QString &title, const IdmContainer &container, const LiquidDb::Storage::Entities &entities, QWidget *parent = 0) :
        ChooseEntityDialog(title, container, entities, parent),
        m_label(tr("Name"), this),
        m_edit(this)
    {
        m_horizontalLayout.setMargin(3);
        m_horizontalLayout.setSpacing(1);
        m_horizontalLayout.addWidget(&m_label);
        m_horizontalLayout.addWidget(&m_edit, 1);

        m_verticatLayout.insertLayout(1, &m_horizontalLayout);
    }

    ::EFC::String name() const { return fromUnicode(m_edit.text()).data(); }

    virtual void accept()
    {
        if (m_edit.text().isEmpty())
            QMessageBox::warning(this, tr("There is empty fields"), tr("Field \"Name\" is empty!"));
        else
            ChooseEntityDialog::accept();
    }

private:
    QHBoxLayout m_horizontalLayout;
    QLabel m_label;
    QLineEdit m_edit;
};


ChooseEntityDialog::ChooseEntityDialog(const QString &title, const IdmContainer &container, const LiquidDb::Storage::Entities &entities, QWidget *parent) :
    QDialog(parent),
    m_container(container),
    m_verticatLayout(this),
    m_label(tr("Entity"), this),
    m_comboBox(this),
    m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
    setWindowTitle(title);

    m_horizontalLayout.setMargin(3);
    m_horizontalLayout.setSpacing(1);
    m_horizontalLayout.addWidget(&m_label);
    m_horizontalLayout.addWidget(&m_comboBox, 1);

    m_verticatLayout.setMargin(3);
    m_verticatLayout.setSpacing(1);
    m_verticatLayout.addLayout(&m_horizontalLayout);
    m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    for (auto i : entities)
        m_comboBox.addItem(toUnicode(i.second.name()), static_cast<qlonglong>(i.second.id()));

    m_comboBox.setCurrentIndex(0);
}

Entity ChooseEntityDialog::chooseFile(const IdmContainer &container, QWidget *parent)
{
    LiquidDb::Storage::Entities entities;

    for (auto i : container.entities())
        if (container.schema(i.second) == IdmContainer::Path)
            entities[i.second.id()] = i.second;

    if (entities.empty())
        QMessageBox::information(parent,
                                 tr("Entities not found"),
                                 tr("There is no entities with a property of type \"%1\"."));
//                                      arg(container.entityTypes().value(Database::Path).label));
    else
        if (entities.size() == 1)
            return (*entities.begin()).second;
        else
        {
            ChooseEntityDialog dialog(tr("Choose entity"), container, entities, parent);

            if (dialog.exec() == ChooseEntityDialog::Accepted)
                return dialog.value();
        }

    return Entity();
}

Entity ChooseEntityDialog::chooseProperty(const IdmContainer &container, const Entity &entity, ::EFC::String &propertyName, QWidget *parent)
{
    Storage::Entities entities(container.entities());
    entities.erase(entity.id());

    for (auto i : entity.properties())
        entities.erase(i.second.entity.id());

    if (entities.empty())
        QMessageBox::information(parent,
                                 tr("Entities not found"),
                                 tr("There is no entities which are not properties of \"%1\".").
                                      arg(toUnicode(entity.name())));
    else
    {
        ChoosePropertyDialog dialog(tr("Choose a property for \"%1\"").arg(toUnicode(entity.name())), container, entities, parent);

        if (dialog.exec() == ChooseEntityDialog::Accepted)
        {
            propertyName = dialog.name();
            return dialog.value();
        }
    }

    return Entity();
}
