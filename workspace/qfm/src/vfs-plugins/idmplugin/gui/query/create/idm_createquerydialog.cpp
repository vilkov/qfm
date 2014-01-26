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
#include "idm_createquerydialog.h"
#include "model/items/idm_queryentitiesmodelitem.h"
#include "../constraint/idm_constraintquerydialog.h"
#include "../../value/list/static/idm_staticvaluelistdialog.h"
#include "../../../constraints/value/idm_valueconstraint.h"
#include "../../../settings/idm_pluginsettings.h"

#include <application.h>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopWidget>


CreateQueryDialog::CreateQueryDialog(const IdmContainer &container, const Entity &entity, QWidget *parent) :
    QDialog(parent),
    m_verticatLayout(this),
    m_splitter(this),
    m_container(container),
    m_entity(entity),
    m_handler1(this),
    m_handler2(this),
    m_toolBar1(this),
    m_view1(&m_handler1, this),
    m_view2(&m_handler2, this),
    m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
    setWindowTitle(tr("Find \"%1\"").arg(toUnicode(m_entity.name())));

    m_toolBar1.addAction(tr("Add constraint"))->setData(AddConstraint);
    m_toolBar2.addAction(tr("Add group"))->setData(AddGroup);
    m_toolBar2.addAction(tr("Remove"))->setData(RemoveGroupOrConstraint);

    connect(&m_toolBar1, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
    connect(&m_toolBar2, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

    m_splitter.addWidget(&m_view1);
    m_splitter.addWidget(&m_view2);

    m_horizontalLayout.setMargin(3);
    m_horizontalLayout.setSpacing(1);
    m_horizontalLayout.addWidget(&m_toolBar1);
    m_horizontalLayout.addWidget(&m_toolBar2);

    m_verticatLayout.setMargin(3);
    m_verticatLayout.setSpacing(1);
    m_verticatLayout.addLayout(&m_horizontalLayout);
    m_verticatLayout.addWidget(&m_splitter);
    m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    for (auto i : m_entity.properties())
        m_model.add(i.second);

    m_view1.setHeaderHidden(true);
    m_view1.setModel(&m_model);

    m_view2.setItemDelegate(&m_delegate2);
    m_view2.setModel(&m_model2);
    m_view2.selectionModel()->setCurrentIndex(m_model2.index(0, 0), QItemSelectionModel::ClearAndSelect);

    m_handler1.registerMouseDoubleClickEventHandler(&CreateQueryDialog::addConstraint);
    m_handler1.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CreateQueryDialog::addConstraint);
    m_view1.setToolTip(tr("INS/Mouse double click - add constraint"));

    m_handler2.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CreateQueryDialog::removeGroupOrConstraint);
    m_view2.setToolTip(tr("DEL - remove"));

    load();
}

CreateQueryDialog::~CreateQueryDialog()
{
    save();
}

BaseConstraint::Holder CreateQueryDialog::constraint()
{
    return m_model2.constraint();
}

void CreateQueryDialog::accept()
{
    QDialog::accept();
}

void CreateQueryDialog::save()
{
    Settings::instance()->createQueryDialog().setGeometry(saveGeometry());
    Settings::instance()->createQueryDialog().setSplitterState(m_splitter.saveState());
    Settings::instance()->createQueryDialog().setColumn1(m_view2.columnWidth(0));
    Settings::instance()->createQueryDialog().setColumn2(m_view2.columnWidth(1));
}

void CreateQueryDialog::load()
{
    QByteArray geometry = Settings::instance()->createQueryDialog().geometry();

    if (geometry.isEmpty())
        setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), Application::instance()->desktop()->availableGeometry()));
    else
        restoreGeometry(geometry);

    geometry = Settings::instance()->createQueryDialog().splitterState();

    if (!geometry.isEmpty())
        m_splitter.restoreState(geometry);

    if (int column = Settings::instance()->createQueryDialog().column1())
        m_view2.setColumnWidth(0, column);

    if (int column = Settings::instance()->createQueryDialog().column2())
        m_view2.setColumnWidth(1, column);
}

void CreateQueryDialog::actionTriggered(QAction *action)
{
    switch (static_cast<ActionId>(action->data().toInt()))
    {
        case AddGroup:
        {
            m_model2.add(currentIndex2());
            break;
        }

        case AddConstraint:
        {
            addConstraint();
            break;
        }

        case RemoveGroupOrConstraint:
        {
            removeGroupOrConstraint();
            break;
        }
    }
}

QModelIndex CreateQueryDialog::currentIndex1()
{
    return m_view1.selectionModel()->currentIndex();
}

QModelIndex CreateQueryDialog::currentIndex2()
{
    return m_view2.selectionModel()->currentIndex();
}

void CreateQueryDialog::addConstraint()
{
    QModelIndex index1 = currentIndex1();

    if (index1.isValid())
    {
        QModelIndex index2 = currentIndex2();

        if (index2.isValid() && static_cast<BaseConstraint *>(index2.internalPointer())->isGroup())
            if (static_cast<QueryEntitiesModelItem *>(index1.internalPointer())->property().entity.type() == Entity::Composite)
            {
                EntityValueReader reader(m_container.entityValues(static_cast<QueryEntitiesModelItem *>(index1.internalPointer())->property().entity));
                StaticValueListDialog dialog(m_container, reader, this);

                if (dialog.exec() == StaticValueListDialog::Accepted)
                {
                    BaseConstraint::Holder constraint(
                            new ValueConstraint(
                                    static_cast<QueryEntitiesModelItem *>(index1.internalPointer())->property(),
                                    Constraint::Equal,
                                    dialog.takeValue(),
                                    static_cast<BaseConstraint *>(index2.internalPointer())));

                    m_model2.add(constraint, index2);
                }
            }
            else
            {
                ConstraintQueryDialog dialog(m_container, static_cast<QueryEntitiesModelItem *>(index1.internalPointer())->property(), this);

                if (dialog.exec() == ConstraintQueryDialog::Accepted)
                    m_model2.add(dialog.takeConstraint(static_cast<BaseConstraint *>(index2.internalPointer())), index2);
            }
        else
            QMessageBox::warning(this, windowTitle(), tr("You must select a destination group!"));
    }
    else
        QMessageBox::warning(this, windowTitle(), tr("You must select a property!"));
}

void CreateQueryDialog::removeGroupOrConstraint()
{
    QModelIndex index2 = currentIndex2();

    if (index2.isValid())
        m_model2.remove(index2);
}
