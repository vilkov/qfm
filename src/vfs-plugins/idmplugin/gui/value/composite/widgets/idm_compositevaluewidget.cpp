#include "idm_compositevaluewidget.h"
#include "../../model/items/idm_compositevaluevalueitem.h"
#include "../../model/items/idm_compositevaluepathitem.h"
#include "../../model/items/idm_compositevaluepropertyitem.h"
#include "../../list/selectable/widgets/idm_selectablevaluelistwidget.h"
#include "../../../../storage/values/idm_valuereader.h"


CompositeValueWidgetPrivate::ICallback::~ICallback()
{}

CompositeValueWidgetPrivate::CompositeValueWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const EntityValue::Holder &value) :
	m_callback(callback),
	m_container(container),
	m_value(value),
	m_view(handler),
	m_model(m_value)
{
	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

CompositeValueWidgetPrivate::CompositeValueWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files) :
	m_callback(callback),
	m_container(container),
	m_value(value),
	m_view(handler),
	m_model(m_value, files)
{
	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

void CompositeValueWidgetPrivate::open(const QModelIndex &index)
{
	static_cast<CompositeValuePathItem *>(index.internalPointer())->open();
}

void CompositeValueWidgetPrivate::addValue(const QModelIndex &index)
{
	Entity *entity = static_cast<CompositeValuePropertyItem *>(index.internalPointer())->entity();
	QByteArray name = Database::savepoint("CompositeValueDialog::doAddValue::");

	if (m_container.savepoint(name))
	{
		SelectableValueListWidget dialog(m_container, Select(entity), m_callback->parent());

		if (dialog.exec() == SelectableValueListWidget::Accepted)
		{
			EntityValue::Holder value;

			if (m_container.addValue(m_value, value = dialog.takeValue()))
				if (m_container.release(name))
					m_model.add(index, value);
				else
				{
					m_container.rollback(name);
					m_callback->critical(m_container.lastError());
				}
			else
			{
				m_container.rollback(name);
				m_callback->critical(m_container.lastError());
			}
		}
		else
			m_container.rollback(name);
	}
	else
		m_callback->critical(m_container.lastError());
}

void CompositeValueWidgetPrivate::removeValue(const QModelIndex &index)
{
	EntityValue::Holder value(static_cast<CompositeValueValueItem *>(index.internalPointer())->value());
	QByteArray name = Database::savepoint("CompositeValueDialog::doRemoveValue::");

	if (m_container.savepoint(name))
		if (m_container.removeValue(m_value, value))
			if (m_container.release(name))
				m_model.remove(index);
			else
			{
				m_container.rollback(name);
				m_callback->critical(m_container.lastError());
			}
		else
		{
			m_container.rollback(name);
			m_callback->critical(m_container.lastError());
		}
	else
		m_callback->critical(m_container.lastError());
}

MainCompositeValueWidget::MainCompositeValueWidget(EventHandler *handler, const IdmContainer &container, const EntityValue::Holder &value, NestedDialog *parent) :
	BaseNestedWidget(parent),
	m_private(this, handler, container, value)
{}

MainCompositeValueWidget::MainCompositeValueWidget(EventHandler *handler, const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files, NestedDialog *parent) :
	BaseNestedWidget(parent),
	m_private(this, handler, container, value, files)
{}

QWidget *MainCompositeValueWidget::centralWidget()
{
	return &m_private.view();
}

void MainCompositeValueWidget::setReadOnly(bool value)
{
	m_private.view().setEnabled(!value);
}

void MainCompositeValueWidget::setFocus()
{
	m_private.view().setFocus();
}

NestedDialog *MainCompositeValueWidget::parent()
{
	return BaseNestedWidget::parent();
}

void MainCompositeValueWidget::critical(const QString &text)
{
	BaseNestedWidget::critical(text);
}

CompositeValueWidget::CompositeValueWidget(const IdmContainer &container, const EntityValue::Holder &value, NestedDialog *parent, const QString &title) :
	NestedWidget(parent, title),
	m_handler(this),
	m_private(this, &m_handler, container, value)
{
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CompositeValueWidget::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CompositeValueWidget::removeValue);
    m_private.view().setToolTip(tr("INS - add value\nDEL - remove value"));

	setCentralWidget(&m_private.view());
}

void CompositeValueWidget::setFocus()
{
	m_private.view().setFocus();
}

NestedDialog *CompositeValueWidget::parent()
{
	return BaseNestedWidget::parent();
}

void CompositeValueWidget::critical(const QString &text)
{
	BaseNestedWidget::critical(text);
}

void CompositeValueWidget::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<CompositeValueItem *>(index.internalPointer())->isProperty())
		m_private.addValue(index);
}

void CompositeValueWidget::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && !static_cast<CompositeValueItem *>(index.internalPointer())->isProperty())
		m_private.removeValue(index);
}
