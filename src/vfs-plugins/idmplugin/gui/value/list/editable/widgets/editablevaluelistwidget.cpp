#include "editablevaluelistwidget.h"
#include "../../../simple/widgets/simplevaluewidget.h"
#include "../../../simple/rating/widgets/ratingvaluewidget.h"
#include "../../../composite/widgets/compositevaluewidget.h"

#include <desktop/theme/desktop_theme.h>


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


EditableValueListWidgetPrivate::ICallback::~ICallback()
{}

EditableValueListWidgetPrivate::EditableValueListWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const Select &query) :
	QWidget(),
	m_callback(callback),
	m_container(container),
	m_entity(query.entity()),
	m_vLayout(this),
	m_handler(this),
	m_filter(&m_handler, this),
	m_search(::Desktop::Theme::current()->searchIcon(), QString(), this),
	m_view(handler, this),
	m_model(m_container, query)
{
	m_vLayout.setMargin(1);
	m_vLayout.setSpacing(1);

	m_hLayout.setMargin(1);
	m_hLayout.setSpacing(1);
	m_hLayout.addWidget(&m_filter);
	m_hLayout.addWidget(&m_search);

	m_view.setHeaderHidden(true);

	m_vLayout.addLayout(&m_hLayout);
	m_vLayout.addWidget(&m_view);

	if (m_model.isValid())
	{
		m_proxy.setDynamicSortFilter(true);
		m_proxy.setSourceModel(&m_model);
		m_view.setSortingEnabled(true);
		m_view.sortByColumn(0, Qt::AscendingOrder);
		m_view.setModel(&m_proxy);
	}
	else
		callback->critical(m_model.lastError());

    connect(&m_search, SIGNAL(clicked()), this, SLOT(setFilter()));
    connect(&m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(selectValue(QModelIndex)));

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Enter, &EditableValueListWidgetPrivate::setFilter);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Return, &EditableValueListWidgetPrivate::setFilter);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Escape, &EditableValueListWidgetPrivate::clearFilter);
    m_filter.setToolTip(tr("ENTER - accept filter\nESC - clear filter, if filter is empty move focus to view"));

    m_search.setToolTip(tr("Accept filter"));
}

void EditableValueListWidgetPrivate::addValue()
{
	bool declined = false;
	QString title = tr("New value for \"%1\"").arg(m_entity->name());
	IdmEntityValue::Holder value;

	switch (m_entity->type())
	{
		case Database::Int:
			value = processAddValue<Database::Int>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::String:
			value = processAddValue<Database::String>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::Date:
			value = processAddValue<Database::Date>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::Time:
			value = processAddValue<Database::Time>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::DateTime:
			value = processAddValue<Database::DateTime>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::Memo:
			value = processAddValue<Database::Memo>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::Composite:
			value = processAddValue<Database::Composite>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::Rating:
			value = processAddValue<Database::Rating>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		case Database::Path:
			value = processAddValue<Database::Path>(m_callback->parent(), title, m_container, m_entity, declined);
			break;

		default:
			break;
	}

	if (value)
	{
		setCurrentIndex(m_model.add(value));
		m_callback->acceptAndClose();
	}
	else
		if (!declined)
			m_callback->critical(m_container.lastError());
}

void EditableValueListWidgetPrivate::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		if (m_container.removeValue(m_entity, IdmStorage::IdsList() << m_model.at(index.row())->id()))
			m_model.remove(index);
		else
			m_callback->critical(m_container.lastError());
}

//void EditableValueListWidgetPrivate::select(const QModelIndex &index)
//{
//	m_view.setFocus();
//	m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
//	m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
//	m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
//}

void EditableValueListWidgetPrivate::setFilter()
{
	m_proxy.setFilter(m_filter.text());

	if (!m_filter.text().isEmpty())
		m_view.setFocus();
}

void EditableValueListWidgetPrivate::clearFilter()
{
	if (m_filter.text().isEmpty())
		m_view.setFocus();
	else
		m_filter.clear();

	m_proxy.setFilter(QString());
}

void EditableValueListWidgetPrivate::selectValue(const QModelIndex &index)
{
	m_filter.setText(m_model.at(m_proxy.mapToSource(index).row())->value().toString());
}

void EditableValueListWidgetPrivate::setCurrentIndex(const QModelIndex &index) const
{
	m_view.selectionModel()->setCurrentIndex(m_proxy.mapFromSource(index), QItemSelectionModel::ClearAndSelect);
}


MainEditableValueListWidget::MainEditableValueListWidget(EventHandler *handler, const IdmContainer &container, const Select &query, NestedDialog *parent) :
	BaseNestedWidget(parent),
	m_private(this, handler, container, query)
{}

QWidget *MainEditableValueListWidget::centralWidget()
{
	return &m_private;
}

void MainEditableValueListWidget::setReadOnly(bool value)
{
	m_private.view().setEnabled(!value);
}

void MainEditableValueListWidget::setFocus()
{
	m_private.setFocusToFilter();
}

void MainEditableValueListWidget::acceptAndClose()
{
	accept();
}

NestedDialog *MainEditableValueListWidget::parent()
{
	return BaseNestedWidget::parent();
}

void MainEditableValueListWidget::critical(const QString &text)
{
	BaseNestedWidget::critical(text);
}


EditableValueListWidget::EditableValueListWidget(const IdmContainer &container, const Select &query, NestedDialog *parent) :
	NestedWidget(parent, tr("Values of \"%1\"").arg(query.entity()->name())),
	m_handler(this),
	m_private(this, &m_handler, container, query)
{
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &EditableValueListWidget::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &EditableValueListWidget::removeValue);
    m_handler.registerShortcut(Qt::CTRL, Qt::Key_F, &EditableValueListWidget::setFocusToFilter);
    m_private.view().setToolTip(tr("INS - add value\nDEL - remove value\nCTRL+F - activate filter field"));

    setCentralWidget(&m_private);
}

void EditableValueListWidget::setFocus()
{
	m_private.setFocusToFilter();
}

void EditableValueListWidget::acceptAndClose()
{
	accept();
}

NestedDialog *EditableValueListWidget::parent()
{
	return NestedWidget::parent();
}

void EditableValueListWidget::critical(const QString &text)
{
	NestedWidget::critical(text);
}

void EditableValueListWidget::addValue()
{
	m_private.addValue();
}

void EditableValueListWidget::removeValue()
{
	m_private.removeValue();
}

void EditableValueListWidget::setFocusToFilter()
{
	m_private.setFocusToFilter();
}
