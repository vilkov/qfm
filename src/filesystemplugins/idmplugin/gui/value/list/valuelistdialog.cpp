#include "valuelistdialog.h"
#include "../../../../../tools/widgets/valuedialog/valuedialog.h"
#include <QtGui/QMessageBox>


/* Meta-function "ValueDialogType" */
template <Database::EntityType EntityType> struct ValueDialogType {};
template <> struct ValueDialogType<Database::Int>      { typedef int       type; };
template <> struct ValueDialogType<Database::String>   { typedef QString   type; };
template <> struct ValueDialogType<Database::Date>     { typedef QDate     type; };
template <> struct ValueDialogType<Database::Time>     { typedef QTime     type; };
template <> struct ValueDialogType<Database::DateTime> { typedef QDateTime type; };
template <> struct ValueDialogType<Database::Memo>     { typedef QString   type; };
template <> struct ValueDialogType<Database::Rating>   { typedef int       type; };


template <Database::EntityType EntityType>
class NewValueDialog : public ValueDialog<typename ValueDialogType<EntityType>::type>
{
public:
	typedef typename ValueDialogType<EntityType>::type value_type;
	typedef ValueDialog<value_type>                    base_class;

public:
	NewValueDialog(const QString &title, const QString &label, QWidget *parent = 0) :
		base_class(title, label, value_type(), parent)
	{}
};


template <Database::EntityType EntityType>
inline bool processAddValue(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, ValueListModel &model)
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
inline bool processAddValue<Database::Memo>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, ValueListModel &model)
{
	return false;
}

template <>
inline bool processAddValue<Database::Rating>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, ValueListModel &model)
{
	return false;
}

template <>
inline bool processAddValue<Database::Path>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, ValueListModel &model)
{
	return false;
}


ValueListDialog::ValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_query(query),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_container.prepare(m_query, m_lastError), this),
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

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &ValueListDialog::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &ValueListDialog::removeValue);

    m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);

	if (!m_model.isValid())
		QMessageBox::critical(this, windowTitle(), m_lastError);
}

void ValueListDialog::addValue()
{
	QString label = tr("Value");
	QString title = tr("New value for \"%1\"").arg(m_query.entity()->name());

	if (m_query.entity()->type() == Database::Composite)
	{

	}
	else
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

void ValueListDialog::removeValue()
{
	QModelIndex index = m_view.selectionModel()->currentIndex();

	if (index.isValid())
		if (m_container.removeValue(m_query.entity(), IdmStorage::IdsList() << static_cast<ValueListItem*>(index.internalPointer())->id()))
			m_model.remove(index);
		else
			QMessageBox::critical(this, windowTitle(), m_container.lastError());
}