#include "compositevaluewidget.h"
#include "../../model/items/compositevaluevalueitem.h"
#include "../../model/items/compositevaluepropertyitem.h"
#include "../../list/selectable/selectablevaluelistdialog.h"
#include "../../../../storage/values/idmvaluereader.h"


CompositeValueWidgetPrivate::ICallback::~ICallback()
{}

CompositeValueWidgetPrivate::CompositeValueWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value) :
	m_callback(callback),
	m_container(container),
	m_value(value),
	m_view(handler),
	m_model(m_value)
{
	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

void CompositeValueWidgetPrivate::addValue(const QModelIndex &index)
{
	IdmEntity *entity = static_cast<CompositeValuePropertyItem*>(index.internalPointer())->entity();
	QByteArray name = Database::savepoint("CompositeValueDialog::doAddValue::");

	if (m_container.savepoint(name))
	{
		SelectableValueListDialog dialog(m_container, Select(entity), m_callback->parent());

		if (dialog.exec() == SelectableValueListDialog::Accepted)
		{
			IdmEntityValue::Holder value;

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
	IdmEntityValue::Holder value(static_cast<CompositeValueValueItem*>(index.internalPointer())->value());
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

MainCompositeValueWidget::MainCompositeValueWidget(EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value, NestedDialog *parent) :
	BaseNestedWidget(parent),
	m_private(this, handler, container, value)
{}

QWidget *MainCompositeValueWidget::centralWidget()
{
	return &m_private.view();
}

void MainCompositeValueWidget::setReadOnly(bool value)
{
	m_private.view().setEnabled(false);
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

CompositeValueWidget::CompositeValueWidget(const IdmContainer &container, const IdmEntityValue::Holder &value, NestedDialog *parent, const QString &title) :
	NestedWidget(parent, title),
	m_handler(this),
	m_private(this, &m_handler, container, value)
{
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CompositeValueWidget::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CompositeValueWidget::removeValue);
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
