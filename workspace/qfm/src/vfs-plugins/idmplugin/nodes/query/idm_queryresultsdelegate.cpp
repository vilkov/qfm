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
#include "idm_queryresultsdelegate.h"
#include "items/idm_queryresultvalueitem.h"
#include "items/idm_queryresultpropertyitem.h"
#include "items/idm_queryresultpathvalueitem.h"
#include "items/idm_queryresultcompositerootitem.h"
#include "../../gui/value/edit/idm_editcompositevaluedialog.h"

#include <tools/widgets/valuedialog/valuedialogmetafunctions.h>

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

QueryResultsDelegate::QueryResultsDelegate(const IdmContainer &container, QObject *parent) :
    Delegate(parent),
    m_container(container)
{}

QWidget *QueryResultsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (static_cast<QueryResultItem *>(index.internalPointer())->isValue())
    {
        QueryResultValueItem *item = static_cast<QueryResultValueItem *>(index.internalPointer());

        switch (item->value().entity().type())
        {
            case Entity::Int:
                return new Editor<typename EntityValueType<Entity::Int>::type>::type(parent);

            case Entity::String:
                return new Editor<typename EntityValueType<Entity::String>::type>::type(parent);

            case Entity::Date:
                return new Editor<typename EntityValueType<Entity::Date>::type>::type(parent);

            case Entity::Time:
                return new Editor<typename EntityValueType<Entity::Time>::type>::type(parent);

            case Entity::DateTime:
                return new Editor<typename EntityValueType<Entity::DateTime>::type>::type(parent);

            case Entity::Memo:
                return new Editor<typename EntityValueType<Entity::Memo>::type>::type(parent);

            case Entity::Composite:
            {
                if (m_container.transaction())
                {
                    EditCompositeValueDialog dialog(m_container, item->value(), parent);

                    if (dialog.exec() != EditCompositeValueDialog::Accepted)
                        m_container.rollback();
                    else
                        if (m_container.commit())
                            CompositeEntityValue(item->value()).resetValue();
                        else
                        {
                            m_container.rollback();
                            QMessageBox::critical(parent, tr("Error"), toUnicode(m_container.lastError()));
                        }
                }
                else
                    QMessageBox::critical(parent, tr("Error"), toUnicode(m_container.lastError()));

                break;
            }

//            case Entity::Rating:
//                return new Editor<typename EntityValueType<Entity::Rating>::type>::type(parent);

//            case Entity::Path:
//                return new Editor<typename EntityValueType<Entity::Path>::type>::type(parent);

            default:
                break;
        }
    }
    else
        if (static_cast<QueryResultItem *>(index.internalPointer())->isProperty())
            return new Editor<QString>::type(parent);

    return NULL;
}

void QueryResultsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (static_cast<QueryResultItem *>(index.internalPointer())->isValue())
    {
        QueryResultValueItem *item = static_cast<QueryResultValueItem *>(index.internalPointer());

        switch (item->value().entity().type())
        {
            case Entity::Int:
                EditorValue<typename EntityValueType<Entity::Int>::type>::setValue(editor, index.data(Qt::DisplayRole));
                break;

            case Entity::String:
                EditorValue<typename EntityValueType<Entity::String>::type>::setValue(editor, index.data(Qt::DisplayRole));
                break;

            case Entity::Date:
                EditorValue<typename EntityValueType<Entity::Date>::type>::setValue(editor, index.data(Qt::DisplayRole));
                break;

            case Entity::Time:
                EditorValue<typename EntityValueType<Entity::Time>::type>::setValue(editor, index.data(Qt::DisplayRole));
                break;

            case Entity::DateTime:
                EditorValue<typename EntityValueType<Entity::DateTime>::type>::setValue(editor, index.data(Qt::DisplayRole));
                break;

            case Entity::Memo:
                EditorValue<typename EntityValueType<Entity::Memo>::type>::setValue(editor, index.data(Qt::DisplayRole));
                break;

//            case Entity::Rating:
//                EditorValue<typename EntityValueType<Entity::Rating>::type>::setValue(editor, index.data(Qt::DisplayRole));
//                break;

//            case Entity::Path:
//                EditorValue<typename EntityValueType<Entity::Path>::type>::setValue(editor, static_cast<QueryResultPathValueItem*>(item)->info().fileName());
//                break;

            case Entity::Composite:
            default:
                break;
        }
    }
    else
        EditorValue<QString>::setValue(editor, toUnicode(static_cast<QueryResultPropertyItem *>(index.internalPointer())->property().name));
}

void QueryResultsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (static_cast<QueryResultItem *>(index.internalPointer())->isValue())
    {
        QueryResultValueItem *item = static_cast<QueryResultValueItem *>(index.internalPointer());
        ::EFC::Variant value;

        switch (item->value().entity().type())
        {
            case Entity::Int:
                value = toVariant(EditorValue<typename EntityValueType<Entity::Int>::type>::value(editor));
                break;

            case Entity::String:
                value = toVariant(EditorValue<typename EntityValueType<Entity::String>::type>::value(editor));
                break;

            case Entity::Date:
                value = toVariant(EditorValue<typename EntityValueType<Entity::Date>::type>::value(editor));
                break;

            case Entity::Time:
                value = toVariant(EditorValue<typename EntityValueType<Entity::Time>::type>::value(editor));
                break;

            case Entity::DateTime:
                value = toVariant(EditorValue<typename EntityValueType<Entity::DateTime>::type>::value(editor));
                break;

            case Entity::Memo:
                value = toVariant(EditorValue<typename EntityValueType<Entity::Memo>::type>::value(editor));
                break;

//            case Entity::Rating:
//                value = EditorValue<typename EntityValueType<Entity::Rating>::type>::value(editor);
//                break;
//
//            case Entity::Path:
//                value = EditorValue<typename EntityValueType<Entity::Path>::type>::value(editor);
//                break;

            case Entity::Composite:
            default:
                break;
        }

//        if (m_container.transaction())
//            if (item->value()->entity()->type() == Entity::Path)
//            {
//                QString error;
//                QueryResultPathValueItem *file = static_cast<QueryResultPathValueItem*>(item);
//                QString fileName = file->info().fileName();
//
//                if (file->info().rename(value.toString(), error))
//                    if (m_container.updateValue(file->value(), file->info().absoluteFilePath(value.toString())))
//                        if (m_container.commit())
//                            file->update();
//                        else
//                        {
//                            m_container.rollback();
//                            file->info().rename(fileName, error);
//                            QMessageBox::critical(editor, tr("Error"), toUnicode(m_container.lastError()));
//                        }
//                    else
//                    {
//                        m_container.rollback();
//                        file->info().rename(fileName, error);
//                        QMessageBox::critical(
//                                    editor,
//                                    tr("Failed to rename file \"%1\"").arg(file->info().fileName()),
//                                    toUnicode(m_container.lastError()));
//                    }
//                else
//                {
//                    m_container.rollback();
//                    QMessageBox::critical(
//                            editor,
//                            tr("Failed to rename file \"%1\"").
//                            arg(file->info().fileName()),
//                            error);
//                }
//            }
//            else
                if (m_container.updateValue(item->value(), value))
                {
                    if (!m_container.commit())
                    {
                        QMessageBox::critical(editor, tr("Error"), toUnicode(m_container.lastError()));
                        m_container.rollback();
                    }
                }
                else
                {
                    QMessageBox::critical(editor, tr("Error"), toUnicode(m_container.lastError()));
                    m_container.rollback();
                }
//        else
//            QMessageBox::critical(editor, tr("Error"), toUnicode(m_container.lastError()));
    }
    else
        if (m_container.transaction())
        {
            QueryResultPropertyItem *property = static_cast<QueryResultPropertyItem *>(index.internalPointer());

            if (m_container.renameProperty(
                    static_cast<QueryResultRootItem *>(property->parent())->value().entity(),
                    property->property().entity,
                    fromUnicode(EditorValue<QString>::value(editor)).data()))
            {
                if (!m_container.commit())
                {
                    QMessageBox::critical(editor, tr("Error"), toUnicode(m_container.lastError()));
                    m_container.rollback();
                }
            }
            else
            {
                QMessageBox::critical(
                            editor,
                            tr("Failed to rename property \"%1\"").arg(toUnicode(property->property().name)),
                            toUnicode(m_container.lastError()));
                m_container.rollback();
            }
        }
        else
            QMessageBox::critical(editor, tr("Error"), toUnicode(m_container.lastError()));
}

IDM_PLUGIN_NS_END
