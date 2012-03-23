#include "editablevaluelistwidget.h"


EditableValueListWidgetPrivate::ICallback::~ICallback()
{}

EditableValueListWidgetPrivate::EditableValueListWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value) :
	m_container(container),
	m_entity(query.entity()),
	m_view(&handler),
	m_model(m_container, query, this)
{
	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}
