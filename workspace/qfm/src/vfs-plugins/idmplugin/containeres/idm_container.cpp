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
#include "idm_container.h"
#include "../idm_plugin.h"

#include <tools/memory/memory_scopedpointer.h>


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


QByteArray fromUnicode(const QString &string)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    return codec->fromUnicode(string);
}

QString toUnicode(const ::EFC::String &string)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    return codec->toUnicode(string.c_str());
}

QVariant toQVariant(const ::EFC::Variant &value)
{
    switch (value.type())
    {
        case ::EFC::Variant::UINT8:
        case ::EFC::Variant::INT8:
        case ::EFC::Variant::UINT16:
        case ::EFC::Variant::INT16:
        case ::EFC::Variant::UINT32:
        case ::EFC::Variant::INT32:
        case ::EFC::Variant::UINT64:
        case ::EFC::Variant::INT64:
        case ::EFC::Variant::BOOL:
        case ::EFC::Variant::CHAR:
            return static_cast<qulonglong>(value.asUint64());

        case ::EFC::Variant::FLOAT:
            return value.asFloat();

        case ::EFC::Variant::DOUBLE:
            return value.asDouble();

        case ::EFC::Variant::STRING:
        case ::EFC::Variant::BINARY:
            return toUnicode(value.asString());
    }

    return QVariant();
}


IdmContainer::IdmContainer(IFileContainer::Holder &container, bool create) :
    m_data(new Data(container, create))
{
    m_data->menuActions.push_back(new QAction(tr("Create"), 0));
    m_data->menuActions.last()->setData(Create);

    m_data->menuActions.push_back(new QAction(tr("Find"), 0));
    m_data->menuActions.last()->setData(Find);

    m_data->menuActions.push_back(new QAction(tr("List"), 0));
    m_data->menuActions.last()->setData(List);

    m_data->entityTypes[Entity::Int]       = EntityTypeDescription(tr("Int"),       tr("Integer type"));
    m_data->entityTypes[Entity::String]    = EntityTypeDescription(tr("String"),    tr("String type (max length is 256 characters)"));
    m_data->entityTypes[Entity::Date]      = EntityTypeDescription(tr("Date"),      tr("Date type"));
    m_data->entityTypes[Entity::Time]      = EntityTypeDescription(tr("Time"),      tr("Time type"));
    m_data->entityTypes[Entity::DateTime]  = EntityTypeDescription(tr("DateTime"),  tr("DateTime type"));
    m_data->entityTypes[Entity::Memo]      = EntityTypeDescription(tr("Memo"),      tr("Memo type"));
    m_data->entityTypes[Entity::Composite] = EntityTypeDescription(tr("Composite"), tr("Entity of this type consists of other entities"));
}

EntityValueReader IdmContainer::entityValues(const Entity &entity) const
{
    return m_data->storage.entityValues(entity);
}

EntityValueReader IdmContainer::entityValues(const Entity &entity, const Constraint &constraint) const
{
    return m_data->storage.entityValues(entity, constraint);
}

IdmContainer::Schemas IdmContainer::schema(const Entity &entity) const
{
    ::EFC::Variant value(m_data->storage.metaPropertyValue(entity, Schema));

    if (value.isValid())
        return static_cast<Schemas>(value.asInt8());

    return Default;
}

bool IdmContainer::setSchema(const Entity &entity, Schemas schema)
{
    return m_data->storage.setMetaPropertyValue(entity, Schema, ::EFC::Variant(schema));
}

QRect IdmContainer::editorGeometry(const Entity &entity) const
{
    ::EFC::Variant value(m_data->storage.metaPropertyValue(entity, EditorGeometry));

    if (value.isValid())
    {
        size_t size;
        const int32_t *data = reinterpret_cast<const int32_t *>(value.asBinary(size));
        return QRect(QPoint(data[0], data[1]), QPoint(data[2], data[3]));
    }

    return QRect();
}

bool IdmContainer::setEditorGeometry(const Entity &entity, const QRect &geometry)
{
    unsigned char buffer[sizeof(int32_t) * 4];
    int32_t *data = reinterpret_cast<int32_t *>(buffer);

    data[0] = geometry.left();
    data[1] = geometry.top();
    data[2] = geometry.right();
    data[3] = geometry.bottom();

    return m_data->storage.setMetaPropertyValue(entity, EditorGeometry, ::EFC::Variant(buffer, sizeof(buffer)));
}

QRect IdmContainer::listGeometry(const Entity &entity) const
{
    ::EFC::Variant value(m_data->storage.metaPropertyValue(entity, ListGeometry));

    if (value.isValid())
    {
        size_t size;
        const int32_t *data = reinterpret_cast<const int32_t *>(value.asBinary(size));
        return QRect(QPoint(data[0], data[1]), QPoint(data[2], data[3]));
    }

    return QRect();
}

bool IdmContainer::setListGeometry(const Entity &entity, const QRect &geometry)
{
    unsigned char buffer[sizeof(int32_t) * 4];
    int32_t *data = reinterpret_cast<int32_t *>(buffer);

    data[0] = geometry.left();
    data[1] = geometry.top();
    data[2] = geometry.right();
    data[3] = geometry.bottom();

    return m_data->storage.setMetaPropertyValue(entity, ListGeometry, ::EFC::Variant(buffer, sizeof(buffer)));
}

Entity IdmContainer::createEntity(Entity::Type type, const ::EFC::String &name, const ::EFC::String &title)
{
    return m_data->storage.createEntity(type, name, title);
}

bool IdmContainer::removeEntity(const Entity &entity)
{
    return m_data->storage.removeEntity(entity);
}

bool IdmContainer::addProperty(const Entity &entity, const Entity &property, const ::EFC::String &name)
{
    return m_data->storage.addProperty(entity, property, name);
}

bool IdmContainer::renameProperty(const Entity &entity, const Entity &property, const ::EFC::String &name)
{
    return m_data->storage.renameProperty(entity, property, name);
}

bool IdmContainer::removeProperty(const Entity &entity, const Entity &property)
{
    return m_data->storage.removeProperty(entity, property);
}

EntityValue IdmContainer::addValue(const Entity &entity) const
{
    return m_data->storage.addValue(entity);
}

bool IdmContainer::addValue(const EntityValue &entityValue, const EntityValue &propertyValue) const
{
    return m_data->storage.addValue(entityValue, propertyValue);
}

bool IdmContainer::addValue(const EntityValue &entityValue, const EntityValue::List &propertyValues) const
{
    return m_data->storage.addValue(entityValue, propertyValues);
}

EntityValue IdmContainer::addValue(const Entity &entity, const ::EFC::Variant &value) const
{
    return m_data->storage.addValue(entity, value);
}

bool IdmContainer::updateValue(const EntityValue &value, const ::EFC::Variant &newValue) const
{
    return m_data->storage.updateValue(value, newValue);
}

bool IdmContainer::removeValue(const Entity &entity, const Entity::IdsList &list) const
{
    return m_data->storage.removeValue(entity, list);
}

bool IdmContainer::removeValue(const EntityValue &entityValue, const EntityValue &propertyValue) const
{
    return m_data->storage.removeValue(entityValue, propertyValue);
}

IdmContainer::Data::Data(IFileContainer::Holder &container, bool create) :
    storage(container->location(Plugin::fileName()).as<QByteArray>().data(), create),
    container(container.take())
{}

IdmContainer::Data::~Data()
{
    qDeleteAll(menuActions);
}

IDM_PLUGIN_NS_END
