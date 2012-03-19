#include "editablevaluelistdialog.h"
#include "../../../tools/idmentityvaluecreationtools.h"


EditableValueListDialog::EditableValueListDialog(const IdmContainer &container, const Select &query, NestedDialog *parent) :
	NestedWidget(tr("Values of \"%1\"").arg(m_entity->name()), parent),
	m_container(container),
	m_entity(query.entity()),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_container, query, this)
{
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &EditableValueListDialog::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &EditableValueListDialog::removeValue);

    m_view.setHeaderHidden(true);

	if (m_model.isValid())
		m_view.setModel(&m_model);
	else
		critical(title(), m_model.lastError());
}

void EditableValueListDialog::setFocus()
{
	m_view.setFocus();
}

IdmEntityValue::Holder EditableValueListDialog::takeValue()
{
	return m_model.take(currentIndex());
}

QModelIndex EditableValueListDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

void EditableValueListDialog::addValue()
{
	bool declined = false;
	IdmEntityValue::Holder value(CreationTools::createValue(
			tr("New value for \"%1\"").arg(m_entity->name()),
			tr("Value"),
			this,
			m_container,
			m_entity,
			declined));

	if (value)
	{
		setCurrentIndex(m_model.add(value));
		QTimer::singleShot(50, this, SLOT(accept()));
	}
	else
		if (!declined)
			critical(title(), m_container.lastError());
}

void EditableValueListDialog::removeValue()
{
	QModelIndex index = m_view.selectionModel()->currentIndex();

	if (index.isValid())
		if (m_container.removeValue(m_entity, IdmStorage::IdsList() << m_model.at(index.row())->id()))
			m_model.remove(index);
		else
			critical(title(), m_container.lastError());
}

void EditableValueListDialog::setCurrentIndex(const QModelIndex &index) const
{
	m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void EditableValueListDialog::select(const QModelIndex &index)
{
	m_view.setFocus();
	m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
	m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}
