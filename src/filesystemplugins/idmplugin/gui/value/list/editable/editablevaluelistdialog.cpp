#include "editablevaluelistdialog.h"
#include "../../new/composite/newcompositevaluedialog.h"
#include "../../../../storage/values/idmentityvalue.h"
#include "../../../../../../tools/widgets/valuedialog/valuedialog.h"
#include <QtGui/QMessageBox>


template <Database::EntityType EntityType>
class NewValueDialog : public ValueDialog<typename EntityValueType<EntityType>::type>
{
public:
	typedef typename EntityValueType<EntityType>::type value_type;
	typedef ValueDialog<value_type>                    base_class;

public:
	NewValueDialog(const QString &title, const QString &label, QWidget *parent = 0) :
		base_class(title, label, value_type(), parent)
	{}
};


template <Database::EntityType EntityType>
inline bool processAddValue(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, EditableValueListModel &model)
{
	typedef NewValueDialog<EntityType> NewValueDialog;
	NewValueDialog dialog(title, label, parent);

	if (dialog.exec() == NewValueDialog::Accepted)
	{
		QVariant value = dialog.value();
		IdmEntity::id_type id = container.addValue(entity, value);

		if (id == IdmEntity::InvalidId)
			return false;
		else
			model.add(id, value);
	}

	return true;
}

template <>
inline bool processAddValue<Database::Memo>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, EditableValueListModel &model)
{
	return false;
}

template <>
inline bool processAddValue<Database::Composite>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, EditableValueListModel &model)
{
	QByteArray name = Database::savepoint("processAddValue<Database::Composite>::");

	if (container.savepoint(name))
	{
		NewCompositeValueDialog dialog(container, entity, parent);

		if (dialog.exec() == NewCompositeValueDialog::Accepted)
			if (IdmEntityValue *value = dialog.value())
				if (container.release(name))
					model.add(value);
				else
				{
					container.rollback(name);
					return false;
				}
			else
			{
				container.rollback(name);
				return false;
			}
		else
			container.rollback(name);
	}
	else
		return false;

	return true;
}

template <>
inline bool processAddValue<Database::Rating>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, EditableValueListModel &model)
{
	return false;
}

template <>
inline bool processAddValue<Database::Path>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, EditableValueListModel &model)
{
	return false;
}


EditableValueListDialog::EditableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_query(query),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_container, m_query, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Values of \"%1\"").arg(query.entity()->name()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &EditableValueListDialog::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &EditableValueListDialog::removeValue);

    m_view.setHeaderHidden(true);

	if (m_model.isValid())
		m_view.setModel(&m_model);
	else
		QMessageBox::critical(this, windowTitle(), m_model.lastError());
}

IdmEntityValue *EditableValueListDialog::takeSelectedValue()
{
	return m_model.take(currentIndex());
}

void EditableValueListDialog::accept()
{
	if (currentIndex().isValid())
		QDialog::accept();
	else
		QMessageBox::warning(this, windowTitle(), "You must select a value.");
}

QModelIndex EditableValueListDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

void EditableValueListDialog::addValue()
{
	QString label = tr("Value");
	QString title = tr("New value for \"%1\"").arg(m_query.entity()->name());

	switch (m_query.entity()->type())
	{
		case Database::Int:
		{
			if (!processAddValue<Database::Int>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::String:
		{
			if (!processAddValue<Database::String>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::Date:
		{
			if (!processAddValue<Database::Date>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::Time:
		{
			if (!processAddValue<Database::Time>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::DateTime:
		{
			if (!processAddValue<Database::DateTime>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::Memo:
		{
			if (!processAddValue<Database::Memo>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::Composite:
		{
			if (!processAddValue<Database::Composite>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::Rating:
		{
			if (!processAddValue<Database::Rating>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
		case Database::Path:
		{
			if (!processAddValue<Database::Path>(title, label, this, m_container, m_query.entity(), m_model))
				QMessageBox::critical(this, windowTitle(), m_container.lastError());

			break;
		}
	}
}

void EditableValueListDialog::removeValue()
{
	QModelIndex index = m_view.selectionModel()->currentIndex();

	if (index.isValid())
		if (m_container.removeValue(m_query.entity(), IdmStorage::IdsList() << static_cast<IdmEntityValue*>(index.internalPointer())->id()))
			m_model.remove(index);
		else
			QMessageBox::critical(this, windowTitle(), m_container.lastError());
}
