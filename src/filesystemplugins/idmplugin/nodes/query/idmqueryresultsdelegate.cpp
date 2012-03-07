#include "idmqueryresultsdelegate.h"
#include "items/idmqueryresultvalueitem.h"
#include "items/idmqueryresultpropertyitem.h"
#include "items/idmqueryresultpathvalueitem.h"
#include "../../gui/value/edit/editcompositevaluedialog.h"
#include "../../../../tools/widgets/valuedialog/valuedialogmetafunctions.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmQueryResultsDelegate::IdmQueryResultsDelegate(const IdmContainer &container, QObject *parent) :
	IdmDelegate(parent),
    m_container(container)
{}

QWidget *IdmQueryResultsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
						if (!m_container.commit())
						{
							m_container.rollback();
							QMessageBox::critical(parent, tr("Error"), m_container.lastError());
						}
				}
				else
					QMessageBox::critical(parent, tr("Error"), m_container.lastError());

				return 0;
			}

			case Database::Rating:
				return new Editor<typename EntityValueType<Database::Rating>::type>::type(parent);

			case Database::Path:
				return new Editor<typename EntityValueType<Database::Path>::type>::type(parent);

			default:
				break;
		}
	}
	else
		if (static_cast<QueryResultItem *>(index.internalPointer())->isProperty())
			return new Editor<QString>::type(parent);

	return 0;
}

void IdmQueryResultsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
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

			case Database::Path:
				EditorValue<typename EntityValueType<Database::Path>::type>::setValue(editor, static_cast<QueryResultPathValueItem*>(item)->info().fileName());
				break;

			default:
				break;
		}
	}
	else
		EditorValue<QString>::setValue(editor, static_cast<QueryResultPropertyItem*>(index.internalPointer())->property().name);
}

void IdmQueryResultsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
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

			default:
				break;
		}

		if (m_container.transaction())
			if (item->value()->entity()->type() == Database::Path)
			{
				QString error;
				QueryResultPathValueItem *file = static_cast<QueryResultPathValueItem*>(item);
				QString fileName = file->info().fileName();

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
				{
					m_container.rollback();
					QMessageBox::critical(
							editor,
							tr("Failed to rename file \"%1\"").
							arg(file->info().fileName()),
							error);
				}
			}
			else
				if (m_container.updateValue(item->value(), value))
				{
					if (!m_container.commit())
					{
						m_container.rollback();
						QMessageBox::critical(editor, tr("Error"), m_container.lastError());
					}
				}
				else
				{
					m_container.rollback();
					QMessageBox::critical(editor, tr("Error"), m_container.lastError());
				}
		else
			QMessageBox::critical(editor, tr("Error"), m_container.lastError());
	}
	else
		if (m_container.transaction())
		{
			QueryResultPropertyItem *property = static_cast<QueryResultPropertyItem*>(index.internalPointer());

			if (m_container.renameProperty(property->rootValue()->entity(), property->property().entity, EditorValue<QString>::value(editor)))
			{
				if (!m_container.commit())
				{
					m_container.rollback();
					QMessageBox::critical(editor, tr("Error"), m_container.lastError());
				}
			}
			else
			{
				m_container.rollback();
				QMessageBox::critical(
							editor,
							tr("Failed to rename property \"%1\"").arg(property->property().name),
							m_container.lastError());
			}
		}
		else
			QMessageBox::critical(editor, tr("Error"), m_container.lastError());
}

IDM_PLUGIN_NS_END
