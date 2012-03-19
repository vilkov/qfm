#include "editablevaluelistdialog.h"
#include "../../simple/widgets/simplevaluewidget.h"
#include "../../simple/rating/widgets/ratingvaluewidget.h"
#include "../../composite/widgets/compositevaluewidget.h"


template <Database::EntityType EntityType>
class NewValueDialog : public SimpleValueWidget<typename EntityValueType<EntityType>::type>
{
public:
	typedef typename EntityValueType<EntityType>::type value_type;
	typedef SimpleValueWidget<value_type>              base_class;

public:
	NewValueDialog(NestedDialog *parent, const QString &title) :
		base_class(parent, title, value_type())
	{}
};


template <Database::EntityType EntityType>
inline IdmEntityValue::Holder processAddValue(NestedDialog *parent, const QString &title, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	typedef NewValueDialog<EntityType> NewValueDialog;
	NewValueDialog dialog(parent, title);

	if (dialog.exec() == NewValueDialog::Accepted)
		return container.addValue(entity, dialog.value());
	else
		declined = true;

	return IdmEntityValue::Holder();
}

template <>
inline IdmEntityValue::Holder processAddValue<Database::Memo>(NestedDialog *parent, const QString &title, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	declined = false;
	return IdmEntityValue::Holder();
}

template <>
inline IdmEntityValue::Holder processAddValue<Database::Composite>(NestedDialog *parent, const QString &title, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	QByteArray name = Database::savepoint("processAddValue<Database::Composite>::");

	if (container.savepoint(name))
	{
		IdmEntityValue::Holder value(container.addValue(entity));

		if (value)
		{
			CompositeValueWidget widget(container, value, parent, title);

			if (widget.exec() == CompositeValueWidget::Accepted)
				if (container.release(name))
					return value;
				else
					container.rollback(name);
			else
			{
				declined = true;
				container.rollback(name);
			}
		}
	}

	return IdmEntityValue::Holder();
}

template <>
inline IdmEntityValue::Holder processAddValue<Database::Rating>(NestedDialog *parent, const QString &title, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	RatingValueWidget widget(parent, title);

	if (widget.exec() == RatingValueWidget::Accepted)
		return container.addValue(entity, widget.value());
	else
		declined = true;

	return IdmEntityValue::Holder();
}

template <>
inline IdmEntityValue::Holder processAddValue<Database::Path>(NestedDialog *parent, const QString &title, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	declined = false;
	return IdmEntityValue::Holder();
}


EditableValueListDialog::EditableValueListDialog(const IdmContainer &container, const Select &query, NestedDialog *parent) :
	NestedWidget(parent, tr("Values of \"%1\"").arg(m_entity->name())),
	m_container(container),
	m_entity(query.entity()),
	m_handler(this),
	m_view(&m_handler),
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
	QString title = tr("New value for \"%1\"").arg(m_entity->name());
	IdmEntityValue::Holder value;

	switch (m_entity->type())
	{
		case Database::Int:
			value = processAddValue<Database::Int>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::String:
			value = processAddValue<Database::String>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::Date:
			value = processAddValue<Database::Date>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::Time:
			value = processAddValue<Database::Time>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::DateTime:
			value = processAddValue<Database::DateTime>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::Memo:
			value = processAddValue<Database::Memo>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::Composite:
			value = processAddValue<Database::Composite>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::Rating:
			value = processAddValue<Database::Rating>(parent(), title, m_container, m_entity, declined);
			break;

		case Database::Path:
			value = processAddValue<Database::Path>(parent(), title, m_container, m_entity, declined);
			break;

		default:
			break;
	}

	if (value)
	{
		setCurrentIndex(m_model.add(value));
		accept();
	}
	else
		if (!declined)
			critical(EditableValueListDialog::title(), m_container.lastError());
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
