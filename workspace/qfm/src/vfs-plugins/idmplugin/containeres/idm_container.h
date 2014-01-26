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
#ifndef IDM_CONTAINER_H_
#define IDM_CONTAINER_H_

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QCoreApplication>
#include <QtGui/QAction>
#include <liquiddb/Storage>
#include <liquiddb/EntityConstraint>
#include <liquiddb/EntityValueReader>

#include <vfs/interfaces/vfs_inodeview.h>
#include <vfs/interfaces/vfs_ifilecontainer.h>

#include "idm_entitytypes.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


QByteArray fromUnicode(const QString &string);
QString toUnicode(const ::EFC::String &string);
QVariant toQVariant(const ::EFC::Variant &value);

/* Meta-function "EntityValueType" */
template <Entity::Type EntityType> struct EntityValueType {};
template <> struct EntityValueType<Entity::Int>      { typedef int       type; };
template <> struct EntityValueType<Entity::String>   { typedef QString   type; };
template <> struct EntityValueType<Entity::Date>     { typedef QDate     type; };
template <> struct EntityValueType<Entity::Time>     { typedef QTime     type; };
template <> struct EntityValueType<Entity::DateTime> { typedef QDateTime type; };
template <> struct EntityValueType<Entity::Memo>     { typedef QString   type; };
//template <> struct EntityValueType<Entity::Rating>   { typedef int       type; };
//template <> struct EntityValueType<Entity::Path>     { typedef QString   type; };

/* Meta-function "EntityValueType" */
inline ::EFC::Variant toVariant(int value) { return ::EFC::Variant(static_cast<uint32_t>(value)); }
inline ::EFC::Variant toVariant(const QString &value) { return fromUnicode(value).data(); }
inline ::EFC::Variant toVariant(const QDate &value) { return ::EFC::Variant(QDateTime(value, QTime()).toTime_t()); }
inline ::EFC::Variant toVariant(const QTime &value) { return ::EFC::Variant(QDateTime(QDate::currentDate(), value).toTime_t()); }
inline ::EFC::Variant toVariant(const QDateTime &value) { return ::EFC::Variant(value.toTime_t()); }



class IdmContainer
{
    Q_DECLARE_TR_FUNCTIONS(IdmContainer)

public:
    enum MenuId
    {
        Create,
        Find,
        List
    };

public:
    IdmContainer(IFileContainer::Holder &container, bool create);

    const IFileContainer *container() const { return m_data->container.data(); }
    const INodeView::MenuActionList &menuActions() const { return m_data->menuActions; }
    const EntityTypes &entityTypes() const { return m_data->entityTypes; }

    /* IdmStorage */
    bool isValid() const { return m_data->storage.isValid(); }
    ::EFC::String lastError() const { return ::EFC::String(); }

    const Storage::Entities entities() const { return m_data->storage.entities(); };

    bool transaction() { return m_data->storage.transaction(); }
    bool commit() { return m_data->storage.commit(); }
    void rollback() { m_data->storage.rollback(); }

    EntityValueReader entityValues(const Entity &entity) const;
    EntityValueReader entityValues(const Entity &entity, const Constraint &constraint) const;

    Entity createEntity(Entity::Type type, const ::EFC::String &name, const ::EFC::String &title);
    bool updateEditorGeometry(const Entity &entity, const QRect &geometry);
    bool updateListGeometry(const Entity &entity, const QRect &geometry);
    bool removeEntity(const Entity &entity);

    bool addProperty(const Entity &entity, const Entity &property, const ::EFC::String &name);
    bool renameProperty(const Entity &entity, const Entity &property, const ::EFC::String &name);
    bool removeProperty(const Entity &entity, const Entity &property);

    EntityValue addValue(const Entity &entity) const;
    bool addValue(const EntityValue &entityValue, const EntityValue &propertyValue) const;
    bool addValue(const EntityValue &entityValue, const EntityValue::List &propertyValues) const;
    EntityValue addValue(const Entity &entity, const ::EFC::Variant &value) const;
    bool updateValue(const EntityValue &value, const ::EFC::Variant &newValue) const;
    bool removeValue(const Entity &entity, const Entity::IdsList &ids) const;
    bool removeValue(const EntityValue &entityValue, const EntityValue &propertyValue) const;

private:
    struct Data : public QSharedData
    {
        Data(IFileContainer::Holder &container, bool create);
        ~Data();

        Storage storage;
        EntityTypes entityTypes;
        IFileContainer::Holder container;
        INodeView::MenuActionList menuActions;
    };

private:
    QExplicitlySharedDataPointer<Data> m_data;
};

IDM_PLUGIN_NS_END

#endif /* IDM_CONTAINER_H_ */
