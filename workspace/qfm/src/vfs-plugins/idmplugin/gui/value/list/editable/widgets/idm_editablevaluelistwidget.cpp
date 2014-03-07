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
#include "idm_editablevaluelistwidget.h"
#include "../../../simple/widgets/idm_simplevaluewidget.h"
#include "../../../simple/rating/widgets/idm_ratingvaluewidget.h"
#include "../../../composite/widgets/idm_compositevaluewidget.h"

#include <desktop/theme/desktop_theme.h>


template <Entity::Type EntityType>
class NewValueDialog : public SimpleValueWidget<typename EntityValueType<EntityType>::type>
{
public:
    typedef typename EntityValueType<EntityType>::type value_type;
    typedef SimpleValueWidget<value_type>              base_class;

public:
    NewValueDialog(NestedDialog *parent, const QString &title) :
        base_class(parent, title, value_type())
    {}
};


template <Entity::Type EntityType>
inline EntityValue defaultProcessAddValue(NestedDialog *parent, const QString &title, IdmContainer &container, const Entity &entity, bool &declined)
{
    typedef NewValueDialog<EntityType> NewValueDialog;
    NewValueDialog dialog(parent, title);

    if (dialog.exec() == NewValueDialog::Accepted)
        return container.addValue(entity, toVariant(dialog.value()));
    else
        declined = true;

    return EntityValue();
}

template <Entity::Type EntityType>
inline EntityValue processAddValue(NestedDialog *parent, const QString &title, IdmContainer &container, const Entity &entity, bool &declined)
{
    return defaultProcessAddValue<EntityType>(parent, title, container, entity, declined);
}

template <>
inline EntityValue processAddValue<Entity::Int>(NestedDialog *parent, const QString &title, IdmContainer &container, const Entity &entity, bool &declined)
{
    if (container.schema(entity) == IdmContainer::Rating)
    {
        RatingValueWidget widget(parent, title);

        if (widget.exec() == RatingValueWidget::Accepted)
            return container.addValue(entity, widget.value());
        else
            declined = true;
    }
    else
        return defaultProcessAddValue<Entity::Int>(parent, title, container, entity, declined);

    return EntityValue();
}

template <>
inline EntityValue processAddValue<Entity::String>(NestedDialog *parent, const QString &title, IdmContainer &container, const Entity &entity, bool &declined)
{
    if (container.schema(entity) == IdmContainer::Path)
    {
        declined = false;
        return EntityValue();
    }
    else
        return defaultProcessAddValue<Entity::String>(parent, title, container, entity, declined);

    return EntityValue();
}

template <>
inline EntityValue processAddValue<Entity::Memo>(NestedDialog *parent, const QString &title, IdmContainer &container, const Entity &entity, bool &declined)
{
    declined = false;
    return EntityValue();
}

template <>
inline EntityValue processAddValue<Entity::Composite>(NestedDialog *parent, const QString &title, IdmContainer &container, const Entity &entity, bool &declined)
{
    if (container.transaction())
    {
        EntityValue value(container.addValue(entity));

        if (value.isValid())
        {
            CompositeValueWidget widget(container, value, parent, title);

            if (widget.exec() == CompositeValueWidget::Accepted)
                if (container.commit())
                    return value;
                else
                    container.rollback();
            else
            {
                declined = true;
                container.rollback();
            }
        }
    }

    return EntityValue();
}


EditableValueListWidgetPrivate::ICallback::~ICallback()
{}

EditableValueListWidgetPrivate::EditableValueListWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const EntityValueReader &reader) :
    QWidget(),
    m_callback(callback),
    m_container(container),
    m_entity(reader.entity()),
    m_vLayout(this),
    m_handler(this),
    m_filter(&m_handler, this),
    m_search(::Desktop::Theme::current()->searchIcon(), QString(), this),
    m_view(handler, this),
    m_model(reader)
{
    m_vLayout.setMargin(1);
    m_vLayout.setSpacing(1);

    m_hLayout.setMargin(1);
    m_hLayout.setSpacing(1);
    m_hLayout.addWidget(&m_filter);
    m_hLayout.addWidget(&m_search);

    m_view.setHeaderHidden(true);

    m_vLayout.addLayout(&m_hLayout);
    m_vLayout.addWidget(&m_view);

    if (m_model.isValid())
    {
        m_proxy.setDynamicSortFilter(true);
        m_proxy.setSourceModel(&m_model);
        m_view.setSortingEnabled(true);
        m_view.sortByColumn(0, Qt::AscendingOrder);
        m_view.setModel(&m_proxy);
    }
    else
        callback->critical(m_model.lastError());

    connect(&m_search, SIGNAL(clicked()), this, SLOT(setFilter()));
    connect(&m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(selectValue(QModelIndex)));

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Enter, &EditableValueListWidgetPrivate::setFilter);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Return, &EditableValueListWidgetPrivate::setFilter);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Escape, &EditableValueListWidgetPrivate::clearFilter);
    m_filter.setToolTip(tr("ENTER - accept filter\nESC - clear filter, if filter is empty move focus to view"));

    m_search.setToolTip(tr("Accept filter"));
}

void EditableValueListWidgetPrivate::addValue()
{
    bool declined = false;
    QString title = tr("New value for \"%1\"").arg(toUnicode(m_entity.name()));
    EntityValue value;

    switch (m_entity.type())
    {
        case Entity::Int:
            value = processAddValue<Entity::Int>(m_callback->parent(), title, m_container, m_entity, declined);
            break;

        case Entity::String:
            value = processAddValue<Entity::String>(m_callback->parent(), title, m_container, m_entity, declined);
            break;

        case Entity::Date:
            value = processAddValue<Entity::Date>(m_callback->parent(), title, m_container, m_entity, declined);
            break;

        case Entity::Time:
            value = processAddValue<Entity::Time>(m_callback->parent(), title, m_container, m_entity, declined);
            break;

        case Entity::DateTime:
            value = processAddValue<Entity::DateTime>(m_callback->parent(), title, m_container, m_entity, declined);
            break;

        case Entity::Memo:
            value = processAddValue<Entity::Memo>(m_callback->parent(), title, m_container, m_entity, declined);
            break;

        case Entity::Composite:
            value = processAddValue<Entity::Composite>(m_callback->parent(), title, m_container, m_entity, declined);
            break;

//        case Entity::Rating:
//            value = processAddValue<Entity::Rating>(m_callback->parent(), title, m_container, m_entity, declined);
//            break;
//
//        case Entity::Path:
//            value = processAddValue<Entity::Path>(m_callback->parent(), title, m_container, m_entity, declined);
//            break;

        default:
            break;
    }

    if (value.isValid())
    {
        setCurrentIndex(m_model.add(value));
        m_callback->acceptAndClose();
    }
    else
        if (!declined)
            m_callback->critical(toUnicode(m_container.lastError()));
}

void EditableValueListWidgetPrivate::removeValue()
{
    QModelIndex index = currentIndex();

    if (index.isValid())
    {
        Entity::IdsList list;
        list.push_back(m_model.at(index.row()).id());

        if (m_container.removeValue(m_entity, list))
            m_model.remove(index);
        else
            m_callback->critical(toUnicode(m_container.lastError()));
    }
}

//void EditableValueListWidgetPrivate::select(const QModelIndex &index)
//{
//    m_view.setFocus();
//    m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
//    m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
//    m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
//}

void EditableValueListWidgetPrivate::setFilter()
{
    m_proxy.setFilter(m_filter.text());

    if (!m_filter.text().isEmpty())
        m_view.setFocus();
}

void EditableValueListWidgetPrivate::clearFilter()
{
    if (m_filter.text().isEmpty())
        m_view.setFocus();
    else
        m_filter.clear();

    m_proxy.setFilter(QString());
}

void EditableValueListWidgetPrivate::selectValue(const QModelIndex &index)
{
    m_filter.setText(toUnicode(m_model.at(m_proxy.mapToSource(index).row()).value().asString()));
}

void EditableValueListWidgetPrivate::setCurrentIndex(const QModelIndex &index) const
{
    m_view.selectionModel()->setCurrentIndex(m_proxy.mapFromSource(index), QItemSelectionModel::ClearAndSelect);
}


MainEditableValueListWidget::MainEditableValueListWidget(EventHandler *handler, const IdmContainer &container, const EntityValueReader &reader, NestedDialog *parent) :
    BaseNestedWidget(parent),
    m_private(this, handler, container, reader)
{}

QWidget *MainEditableValueListWidget::centralWidget()
{
    return &m_private;
}

void MainEditableValueListWidget::setReadOnly(bool value)
{
    m_private.view().setEnabled(!value);
}

void MainEditableValueListWidget::setFocus()
{
    m_private.setFocusToFilter();
}

void MainEditableValueListWidget::acceptAndClose()
{
    accept();
}

NestedDialog *MainEditableValueListWidget::parent()
{
    return BaseNestedWidget::parent();
}

void MainEditableValueListWidget::critical(const QString &text)
{
    BaseNestedWidget::critical(text);
}


EditableValueListWidget::EditableValueListWidget(const IdmContainer &container, const EntityValueReader &reader, NestedDialog *parent) :
    NestedWidget(parent, tr("Values of \"%1\"").arg(toUnicode(reader.entity().name()))),
    m_handler(this),
    m_private(this, &m_handler, container, reader)
{
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &EditableValueListWidget::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &EditableValueListWidget::removeValue);
    m_handler.registerShortcut(Qt::CTRL, Qt::Key_F, &EditableValueListWidget::setFocusToFilter);
    m_private.view().setToolTip(tr("INS - add value\nDEL - remove value\nCTRL+F - activate filter field"));

    setCentralWidget(&m_private);
}

void EditableValueListWidget::setFocus()
{
    m_private.setFocusToFilter();
}

void EditableValueListWidget::acceptAndClose()
{
    accept();
}

NestedDialog *EditableValueListWidget::parent()
{
    return NestedWidget::parent();
}

void EditableValueListWidget::critical(const QString &text)
{
    NestedWidget::critical(text);
}

void EditableValueListWidget::addValue()
{
    m_private.addValue();
}

void EditableValueListWidget::removeValue()
{
    m_private.removeValue();
}

void EditableValueListWidget::setFocusToFilter()
{
    m_private.setFocusToFilter();
}
