#include "newcompositevaluedialog.h"
#include "../../list/valuelistdialog.h"
#include "../../../../storage/values/idmvaluereader.h"
#include "../../../../../../tools/pointers/pscopedpointer.h"
#include <QtGui/QMessageBox>


NewCompositeValueDialog::NewCompositeValueDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_entity(entity),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_entity, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("New value for \"%1\"").arg(m_entity->name()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

	connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &NewCompositeValueDialog::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &NewCompositeValueDialog::removeValue);

	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

void NewCompositeValueDialog::accept()
{
	QDialog::accept();
}

IdmEntityValue *NewCompositeValueDialog::value()
{
	IdmContainer::id_type id = m_container.addValue(m_entity);

	if (id != IdmContainer::InvalidId)
	{
		IdmEntityValue *tmp;
		IdmContainer::IdsMap res;
		IdmEntityPropertyItem *property;
		PScopedPointer<IdmEntityValue> value(IdmValueReader::createValue(m_entity, id));

		for (CompositeValueModel::size_type i = 0, size = m_model.size(); i < size; ++i)
		{
			IdmContainer::IdsList &list = res[(property = static_cast<IdmEntityPropertyItem*>(m_model.at(i)))->entity()];
			list.reserve(property->size());

			for (CompositeValueModel::size_type q = 0, size = property->size(); q < size; ++q)
			{
				IdmValueReader::addValue(value.data(), tmp = static_cast<IdmEntityValueItem*>(property->at(q))->take());
				list.push_back(tmp->id());
			}
		}

		if (m_container.addValue(m_entity, id, res))
			return value.take();
	}

	return 0;
}

QModelIndex NewCompositeValueDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

void NewCompositeValueDialog::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		doAddValue(index);
}

void NewCompositeValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<IdmItem*>(index.internalPointer())->isValueItem())
		doRemoveValue(index);
}

void NewCompositeValueDialog::doAddValue(const QModelIndex &index)
{
	IdmEntity *entity = static_cast<IdmEntityItem*>(index.internalPointer())->entity();
	QByteArray name = Database::savepoint("NewCompositeValueDialog::doAddValue::");

	if (m_container.savepoint(name))
	{
		ValueListDialog dialog(m_container, Select(entity), this);

		if (dialog.exec() == ValueListDialog::Accepted)
			if (m_container.release(name))
				m_model.add(index, dialog.takeSelectedValue());
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

void NewCompositeValueDialog::doRemoveValue(const QModelIndex &index)
{
	IdmEntity *entity = static_cast<IdmEntityItem*>(index.internalPointer())->entity();
	QByteArray name = Database::savepoint("NewCompositeValueDialog::doRemoveValue::");

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
