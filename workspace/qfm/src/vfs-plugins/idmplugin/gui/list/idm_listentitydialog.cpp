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
#include "idm_listentitydialog.h"
#include "model/items/idm_entitiestreeitem.h"
#include "../create/idm_createentitydialog.h"

#include <QtGui/QMessageBox>


ListEntityDialog::ListEntityDialog(const IdmContainer &container, QWidget *parent) :
    QDialog(parent),
    m_container(container),
    m_handler(this),
    m_toolBar(this),
    m_view(&m_handler, this),
    m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
    m_verticatLayout(this)
{
    setWindowTitle(tr("List of all entities"));

    m_toolBar.addAction(tr("Create entity"))->setData(Create);
    m_toolBar.addAction(tr("Remove entity"))->setData(Remove);
    m_toolBar.addAction(tr("Add property"))->setData(AddProperty);
    m_toolBar.addAction(tr("Remove property"))->setData(RemoveProperty);

    connect(&m_toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

    m_verticatLayout.setMargin(3);
    m_verticatLayout.setSpacing(1);
    m_verticatLayout.addWidget(&m_toolBar);
    m_verticatLayout.addWidget(&m_view);
    m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    for (auto i : m_container.entities())
        m_model.add(i.second);

    m_view.setHeaderHidden(true);
    m_view.setModel(&m_model);

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &ListEntityDialog::removeEntity);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &ListEntityDialog::insertProperty);
}

void ListEntityDialog::accept()
{
    QDialog::accept();
}

void ListEntityDialog::actionTriggered(QAction *action)
{
    switch (static_cast<ActionId>(action->data().toInt()))
    {
        case Create:
        {
            createEntity();
            break;
        }
        case Remove:
        {
            removeEntity(currentIndex());
            break;
        }
        case AddProperty:
        {
            break;
        }
        case RemoveProperty:
        {
            break;
        }
    }
}

void ListEntityDialog::removeEntity(const QModelIndex &index)
{
    if (m_container.transaction())
    {
        if (QMessageBox::question(this,
                              tr("Remove entity"),
                              tr("Do you really want to remove entity \"%1\"?").
                              arg(toUnicode(static_cast<IdmEntitiesTreeItem*>(index.internalPointer())->entity().name())),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            if (m_container.removeEntity(static_cast<IdmEntitiesTreeItem*>(index.internalPointer())->entity()))
                if (m_container.commit())
                    m_model.remove(index);
                else
                {
                    m_container.rollback();
                    QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
                }
            else
            {
                m_container.rollback();
                QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
            }
    }
    else
        QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
}

void ListEntityDialog::removeEntity()
{
    QMessageBox::information(this, QString(), tr("removeEntity"));
}

void ListEntityDialog::insertProperty()
{
    QMessageBox::information(this, QString(), tr("insertProperty"));
}

void ListEntityDialog::createEntity()
{
    CreateEntityDialog dialog(m_container, QString(), this);

    if (dialog.exec() == CreateEntityDialog::Accepted)
    {
        if (m_container.transaction())
        {
            Entity entity(m_container.createEntity(dialog.type(), dialog.name(), dialog.title()));

            if (entity.isValid())
                if (entity.type() == Entity::Composite)
                {
                    bool ok = true;

                    for (CreateEntityDialog::size_type i = 0, size = dialog.size(); i < size; ++i)
                        if (!m_container.addProperty(entity, dialog.property(i), dialog.propertyName(i)))
                        {
                            ok = false;
                            QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
                            break;
                        }

                    if (ok)
                        if (m_container.commit())
                            m_model.add(entity);
                        else
                        {
                            m_container.rollback();
                            QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
                        }
                }
                else
                    if (m_container.commit())
                        m_model.add(entity);
                    else
                    {
                        m_container.rollback();
                        QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
                    }
            else
            {
                m_container.rollback();
                QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
            }
        }
        else
            QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
    }
}

QModelIndex ListEntityDialog::currentIndex()
{
    return m_view.selectionModel()->currentIndex();
}

void removeEntity()
{
    //    if (item->isEntityItem())
    //        if (item->parent()->isEntityItem())
    //        {
    //            if (QMessageBox::question(this,
    //                                  tr("Remove property"),
    //                                  tr("Do you really want to remove property \"%1\" of entity \"%2\"?").
    //                                  arg(static_cast<IdmEntityItem*>(item)->entity()->name()).
    //                                  arg(static_cast<IdmEntityItem*>(item->parent())->entity()->name()),
    //                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    //                if (m_container.removeProperty(static_cast<IdmEntityItem*>(item->parent())->entity(),
    //                                                static_cast<IdmEntityItem*>(item)->entity()))
    //                {
    //                    IdmMenuEntities *entities = static_cast<IdmMenuEntities*>(m_container.menu()->at(IdmContainer::List));
    //
    //                    beginRemoveRows(parent(idx), idx.row(), idx.row());
    //                    entities->remove(static_cast<IdmEntityItem*>(item), idx.row());
    //                    delete item;
    //                    endRemoveRows();
    //                }
    //                else
    //                {
    //                    QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
    //                    return false;
    //                }
    //        }
    //        else
    //        {
    //            if (QMessageBox::question(this,
    //                                  tr("Remove entity"),
    //                                  tr("Do you really want to remove entity \"%1\"?").
    //                                  arg(static_cast<IdmEntityItem*>(item)->entity()->name()),
    //                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    //                if (m_container.removeEntity(static_cast<IdmEntityItem*>(item)->entity()))
    //                {
    //                    Idmconst Entity &entity = static_cast<IdmEntityItem*>(item)->entity();
    //                    IdmEntityItem *parent;
    //                    IdmMenuEntities *entities = static_cast<IdmMenuEntities*>(m_container.menu()->at(IdmContainer::List));
    //                    IdmMenuEntities::List items = entities->items(entity);
    //                    IdmItemsList::size_type index;
    //
    //                    for (IdmMenuEntities::List::size_type i = 0, size = items.size(); i < size; ++i)
    //                    {
    //                        parent = static_cast<IdmEntityItem*>(items.at(i)->parent());
    //                        index = parent->indexOf(items.at(i));
    //
    //                        beginRemoveRows(IdmNodeBase::index(parent), index, index);
    //                        entities->remove(items.at(i), index);
    //                        delete items.at(i);
    //                        endRemoveRows();
    //                    }
    //
    //                    delete entity;
    //                }
    //                else
    //                {
    //                    QMessageBox::critical(this, tr("Error"), toUnicode(m_container.lastError()));
    //                    return false;
    //                }
    //        }
}
