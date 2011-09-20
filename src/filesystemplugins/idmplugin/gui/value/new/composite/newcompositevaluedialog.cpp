#include "newcompositevaluedialog.h"
#include "../../list/valuelistdialog.h"
#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>


NewCompositeValueDialog::NewCompositeValueDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_entity(entity),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(entity, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("New value for \"%1\"").arg(entity->name()));

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
	CompositeValueModel::Map vals = m_model.entities();
	IdmContainer::IdsMap res;

	for (CompositeValueModel::Map::const_iterator it = vals.constBegin(), end = vals.constEnd(); it != end; ++it)
	{
		const CompositeValueModel::List &source = it.value();
		IdmContainer::IdsList &list = res[it.key()];
		list.reserve(source.size());

		for (CompositeValueModel::List::size_type i = 0, size = source.size(); i < size; ++i)
			list.push_back(static_cast<IdmEntityValueItem*>(source.at(i))->id());
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
	QByteArray name("NewCompositeValueDialog::doAddValue::");
	name.append(QDateTime::currentDateTime().toString(QString::fromLatin1("hh:mm:ss")).toUtf8());

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
	QByteArray name("NewCompositeValueDialog::doRemoveValue::");
	name.append(QDateTime::currentDateTime().toString(QString::fromLatin1("hh:mm:ss")).toUtf8());

	if (m_container.savepoint(name))
		if (m_container.removeValue(entity, IdmStorage::IdsList() << static_cast<IdmEntityValueItem*>(index.internalPointer())->id()))
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
