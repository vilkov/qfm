#include "editcompositevaluedialog.h"
#include "../list/editable/editablevaluelistdialog.h"
#include "../../../items/idmentityvalueitem.h"
#include "../../../items/idmentitypropertyitem.h"
#include "../../../storage/values/idmvaluereader.h"
#include "../../../../../tools/pointers/pscopedpointer.h"
#include <QtGui/QMessageBox>


EditCompositeValueDialog::EditCompositeValueDialog(const IdmContainer &container, IdmCompositeEntityValue *value, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_value(value),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_value, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Editing \"%1\"").arg(m_value->value().toString()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

	connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &EditCompositeValueDialog::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &EditCompositeValueDialog::removeValue);

	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

void EditCompositeValueDialog::accept()
{
	QDialog::accept();
}

IdmEntityValue *EditCompositeValueDialog::value()
{
//	IdmContainer::id_type id = m_container.addValue(m_entity);
//
//	if (id != IdmContainer::InvalidId)
//	{
//		IdmEntityValue *tmp;
//		IdmContainer::IdsMap res;
//		IdmEntityPropertyItem *property;
//		PScopedPointer<IdmEntityValue> value(IdmValueReader::createValue(m_entity, id));
//
//		for (CompositeValueModel::size_type i = 0, size = m_model.size(); i < size; ++i)
//		{
//			IdmContainer::IdsList &list = res[(property = static_cast<IdmEntityPropertyItem*>(m_model.at(i)))->entity()];
//			list.reserve(property->size());
//
//			for (CompositeValueModel::size_type q = 0, size = property->size(); q < size; ++q)
//			{
//				IdmValueReader::addValue(value.data(), tmp = static_cast<IdmEntityValueItem*>(property->at(q))->take());
//				list.push_back(tmp->id());
//			}
//		}
//
//		if (m_container.addValue(m_entity, id, res))
//			return value.take();
//	}

	return 0;
}

QModelIndex EditCompositeValueDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

void EditCompositeValueDialog::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		doAddValue(index);
}

void EditCompositeValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<IBaseItem*>(index.internalPointer())->isValueItem())
		doRemoveValue(index);
}

void EditCompositeValueDialog::doAddValue(const QModelIndex &index)
{
	IdmEntity *entity = static_cast<IdmEntityItem*>(index.internalPointer())->entity();
	QByteArray name = Database::savepoint("EditCompositeValueDialog::doAddValue::");

	if (m_container.savepoint(name))
	{
		EditableValueListDialog dialog(m_container, Select(entity), this);

		if (dialog.exec() == EditableValueListDialog::Accepted)
			if (m_container.release(name))
				m_model.add(index, dialog.takeValue());
			else
			{
				m_container.rollback(name);
				QMessageBox::critical(this, windowTitle(), m_container.lastError());
			}
		else
			m_container.rollback(name);
	}
	else
		QMessageBox::critical(this, windowTitle(), m_container.lastError());
}

void EditCompositeValueDialog::doRemoveValue(const QModelIndex &index)
{
	IdmEntity *entity = static_cast<IdmEntityItem*>(index.internalPointer())->entity();
	QByteArray name = Database::savepoint("EditCompositeValueDialog::doRemoveValue::");

	if (m_container.savepoint(name))
		if (m_container.removeValue(entity, IdmStorage::IdsList() << static_cast<IdmEntityValueItem*>(index.internalPointer())->value()->id()))
			if (m_container.release(name))
				m_model.remove(index);
			else
			{
				m_container.rollback(name);
				QMessageBox::critical(this, windowTitle(), m_container.lastError());
			}
		else
		{
			m_container.rollback(name);
			QMessageBox::critical(this, windowTitle(), m_container.lastError());
		}
	else
		QMessageBox::critical(this, windowTitle(), m_container.lastError());
}
