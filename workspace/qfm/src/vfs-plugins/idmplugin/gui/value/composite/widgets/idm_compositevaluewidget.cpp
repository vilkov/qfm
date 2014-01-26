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
#include "idm_compositevaluewidget.h"
#include "../../model/items/idm_compositevaluevalueitem.h"
#include "../../model/items/idm_compositevaluepathitem.h"
#include "../../model/items/idm_compositevaluepropertyitem.h"
#include "../../list/selectable/widgets/idm_selectablevaluelistwidget.h"

#include <liquiddb/EntityValueReader>


CompositeValueWidgetPrivate::ICallback::~ICallback()
{}

CompositeValueWidgetPrivate::CompositeValueWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const EntityValue &value) :
    m_callback(callback),
    m_container(container),
    m_value(value),
    m_view(handler),
    m_model(m_value),
    m_delegate(m_value, m_container)
{
    m_view.setHeaderHidden(true);
    m_view.setModel(&m_model);
    m_view.setItemDelegate(&m_delegate);
}

CompositeValueWidgetPrivate::CompositeValueWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const EntityValue &value, const CompositeValueModel::Files &files) :
    m_callback(callback),
    m_container(container),
    m_value(value),
    m_view(handler),
    m_model(m_value, files),
    m_delegate(m_value, m_container)
{
    m_view.setHeaderHidden(true);
    m_view.setModel(&m_model);
    m_view.setItemDelegate(&m_delegate);
}

void CompositeValueWidgetPrivate::open(const QModelIndex &index)
{
    static_cast<CompositeValuePathItem *>(index.internalPointer())->open();
}

void CompositeValueWidgetPrivate::addValue(const QModelIndex &index)
{
    const Entity &entity = static_cast<CompositeValuePropertyItem *>(index.internalPointer())->entity();

    if (m_container.transaction())
    {
        EntityValueReader reader(m_container.entityValues(entity));
        SelectableValueListWidget dialog(m_container, reader, m_callback->parent());

        if (dialog.exec() == SelectableValueListWidget::Accepted)
        {
            EntityValue value;

            if (m_container.addValue(m_value, value = dialog.takeValue()))
                if (m_container.commit())
                    m_model.add(index, value);
                else
                {
                    m_container.rollback();
                    m_callback->critical(toUnicode(m_container.lastError()));
                }
            else
            {
                m_container.rollback();
                m_callback->critical(toUnicode(m_container.lastError()));
            }
        }
        else
            m_container.rollback();
    }
    else
        m_callback->critical(toUnicode(m_container.lastError()));
}

void CompositeValueWidgetPrivate::removeValue(const QModelIndex &index)
{
    EntityValue value(static_cast<CompositeValueValueItem *>(index.internalPointer())->value());

    if (m_container.transaction())
        if (m_container.removeValue(m_value, value))
            if (m_container.commit())
                m_model.remove(index);
            else
            {
                m_container.rollback();
                m_callback->critical(toUnicode(m_container.lastError()));
            }
        else
        {
            m_container.rollback();
            m_callback->critical(toUnicode(m_container.lastError()));
        }
    else
        m_callback->critical(toUnicode(m_container.lastError()));
}

MainCompositeValueWidget::MainCompositeValueWidget(EventHandler *handler, const IdmContainer &container, const EntityValue &value, NestedDialog *parent) :
    BaseNestedWidget(parent),
    m_private(this, handler, container, value)
{}

MainCompositeValueWidget::MainCompositeValueWidget(EventHandler *handler, const IdmContainer &container, const EntityValue &value, const CompositeValueModel::Files &files, NestedDialog *parent) :
    BaseNestedWidget(parent),
    m_private(this, handler, container, value, files)
{}

QWidget *MainCompositeValueWidget::centralWidget()
{
    return &m_private.view();
}

void MainCompositeValueWidget::setReadOnly(bool value)
{
    m_private.view().setEnabled(!value);
}

void MainCompositeValueWidget::setFocus()
{
    m_private.view().setFocus();
}

NestedDialog *MainCompositeValueWidget::parent()
{
    return BaseNestedWidget::parent();
}

void MainCompositeValueWidget::critical(const QString &text)
{
    BaseNestedWidget::critical(text);
}

CompositeValueWidget::CompositeValueWidget(const IdmContainer &container, const EntityValue &value, NestedDialog *parent, const QString &title) :
    NestedWidget(parent, title),
    m_handler(this),
    m_private(this, &m_handler, container, value)
{
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CompositeValueWidget::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CompositeValueWidget::removeValue);
    m_private.view().setToolTip(tr("INS - add value\nDEL - remove value"));

    setCentralWidget(&m_private.view());
}

void CompositeValueWidget::setFocus()
{
    m_private.view().setFocus();
}

NestedDialog *CompositeValueWidget::parent()
{
    return BaseNestedWidget::parent();
}

void CompositeValueWidget::critical(const QString &text)
{
    BaseNestedWidget::critical(text);
}

void CompositeValueWidget::addValue()
{
    QModelIndex index = currentIndex();

    if (index.isValid() && static_cast<CompositeValueItem *>(index.internalPointer())->isProperty())
        m_private.addValue(index);
}

void CompositeValueWidget::removeValue()
{
    QModelIndex index = currentIndex();

    if (index.isValid() && !static_cast<CompositeValueItem *>(index.internalPointer())->isProperty())
        m_private.removeValue(index);
}
