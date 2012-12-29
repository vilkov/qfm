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

		switch (item->value()->entity()->type())
		{
			case Database::Int:
				return new Editor<typename EntityValueType<Database::Int>::type>::type(parent);

			case Database::String:
				return new Editor<typename EntityValueType<Database::String>::type>::type(parent);

			case Database::Date:
				return new Editor<typename EntityValueType<Database::Date>::type>::type(parent);

			case Database::Time:
				return new Editor<typename EntityValueType<Database::Time>::type>::type(parent);

			case Database::DateTime:
				return new Editor<typename EntityValueType<Database::DateTime>::type>::type(parent);

			case Database::Memo:
				return new Editor<typename EntityValueType<Database::Memo>::type>::type(parent);

			case Database::Composite:
			{
				if (m_container.transaction())
				{
					EditCompositeValueDialog dialog(m_container, item->value(), parent);

					if (dialog.exec() != EditCompositeValueDialog::Accepted)
						m_container.rollback();
					else
						if (m_container.commit())
						    item->value().as<CompositeEntityValue>()->resetValue();
						else
						{
							m_container.rollback();
							QMessageBox::critical(parent, tr("Error"), m_container.lastError());
						}
				}
				else
					QMessageBox::critical(parent, tr("Error"), m_container.lastError());

				break;
			}

			case Database::Rating:
				return new Editor<typename EntityValueType<Database::Rating>::type>::type(parent);

//			case Database::Path:
//				return new Editor<typename EntityValueType<Database::Path>::type>::type(parent);

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

		switch (item->value()->entity()->type())
		{
			case Database::Int:
				EditorValue<typename EntityValueType<Database::Int>::type>::setValue(editor, index.data(Qt::DisplayRole));
				break;

			case Database::String:
				EditorValue<typename EntityValueType<Database::String>::type>::setValue(editor, index.data(Qt::DisplayRole));
				break;

			case Database::Date:
				EditorValue<typename EntityValueType<Database::Date>::type>::setValue(editor, index.data(Qt::DisplayRole));
				break;

			case Database::Time:
				EditorValue<typename EntityValueType<Database::Time>::type>::setValue(editor, index.data(Qt::DisplayRole));
				break;

			case Database::DateTime:
				EditorValue<typename EntityValueType<Database::DateTime>::type>::setValue(editor, index.data(Qt::DisplayRole));
				break;

			case Database::Memo:
				EditorValue<typename EntityValueType<Database::Memo>::type>::setValue(editor, index.data(Qt::DisplayRole));
				break;

			case Database::Rating:
				EditorValue<typename EntityValueType<Database::Rating>::type>::setValue(editor, index.data(Qt::DisplayRole));
				break;

//			case Database::Path:
//				EditorValue<typename EntityValueType<Database::Path>::type>::setValue(editor, static_cast<QueryResultPathValueItem*>(item)->info().fileName());
//				break;

            case Database::Composite:
			default:
				break;
		}
	}
	else
		EditorValue<QString>::setValue(editor, static_cast<QueryResultPropertyItem *>(index.internalPointer())->property().name);
}

void QueryResultsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (static_cast<QueryResultItem *>(index.internalPointer())->isValue())
	{
		QueryResultValueItem *item = static_cast<QueryResultValueItem *>(index.internalPointer());
		QVariant value;

		switch (item->value()->entity()->type())
		{
			case Database::Int:
				value = EditorValue<typename EntityValueType<Database::Int>::type>::value(editor);
				break;

			case Database::String:
				value = EditorValue<typename EntityValueType<Database::String>::type>::value(editor);
				break;

			case Database::Date:
				value = EditorValue<typename EntityValueType<Database::Date>::type>::value(editor);
				break;

			case Database::Time:
				value = EditorValue<typename EntityValueType<Database::Time>::type>::value(editor);
				break;

			case Database::DateTime:
				value = EditorValue<typename EntityValueType<Database::DateTime>::type>::value(editor);
				break;

			case Database::Memo:
				value = EditorValue<typename EntityValueType<Database::Memo>::type>::value(editor);
				break;

			case Database::Rating:
				value = EditorValue<typename EntityValueType<Database::Rating>::type>::value(editor);
				break;

			case Database::Path:
				value = EditorValue<typename EntityValueType<Database::Path>::type>::value(editor);
				break;

            case Database::Composite:
			default:
				break;
		}

		if (m_container.transaction())
			if (item->value()->entity()->type() == Database::Path)
			{
//				QString error;
//				QueryResultPathValueItem *file = static_cast<QueryResultPathValueItem*>(item);
//				QString fileName = file->info().fileName();
//
//				if (file->info().rename(value.toString(), error))
//					if (m_container.updateValue(file->value(), file->info().absoluteFilePath(value.toString())))
//						if (m_container.commit())
//							file->update();
//						else
//						{
//							m_container.rollback();
//							file->info().rename(fileName, error);
//							QMessageBox::critical(editor, tr("Error"), m_container.lastError());
//						}
//					else
//					{
//						m_container.rollback();
//						file->info().rename(fileName, error);
//						QMessageBox::critical(
//									editor,
//									tr("Failed to rename file \"%1\"").arg(file->info().fileName()),
//									m_container.lastError());
//					}
//				else
//				{
//					m_container.rollback();
//					QMessageBox::critical(
//							editor,
//							tr("Failed to rename file \"%1\"").
//							arg(file->info().fileName()),
//							error);
//				}
			}
			else
				if (m_container.updateValue(item->value(), value))
				{
					if (!m_container.commit())
					{
						QMessageBox::critical(editor, tr("Error"), m_container.lastError());
						m_container.rollback();
					}
				}
				else
				{
					QMessageBox::critical(editor, tr("Error"), m_container.lastError());
					m_container.rollback();
				}
		else
			QMessageBox::critical(editor, tr("Error"), m_container.lastError());
	}
	else
		if (m_container.transaction())
		{
			QueryResultPropertyItem *property = static_cast<QueryResultPropertyItem *>(index.internalPointer());

			if (m_container.renameProperty(
					static_cast<QueryResultRootItem *>(property->parent())->value()->entity(),
					property->property().entity,
					EditorValue<QString>::value(editor)))
			{
				if (!m_container.commit())
				{
					QMessageBox::critical(editor, tr("Error"), m_container.lastError());
					m_container.rollback();
				}
			}
			else
			{
				QMessageBox::critical(
							editor,
							tr("Failed to rename property \"%1\"").arg(property->property().name),
							m_container.lastError());
				m_container.rollback();
			}
		}
		else
			QMessageBox::critical(editor, tr("Error"), m_container.lastError());
}

IDM_PLUGIN_NS_END
