#include "listconfigoption.h"


ListConfigOption::ListConfigOption(const qint32 &id, const QString &title, ConfigOption *parent) :
	ConfigOptionValueDefault(id, title, QStringList() << QString::fromLatin1("http://sget9.avp.ru:8080/?action=feed"), parent),
	m_widget(0),
	m_view(0),
	m_model(0),
	m_listener(0),
	m_addSlot(0),
	m_removeSlot(0)
{}

bool ListConfigOption::isEnabled() const
{
	return m_widget->isEnabled();
}

void ListConfigOption::setEnabled(bool value)
{
	m_widget->setEnabled(value);
}

QWidget *ListConfigOption::createEditor(QWidget *parent)
{
	QScopedPointer<QWidget> widget(m_widget = new QWidget(parent));
	QVBoxLayout *vertical = new QVBoxLayout(widget.data());
	QHBoxLayout *horizontal = new QHBoxLayout();
	QPushButton *button;

	horizontal->setMargin(1);
	horizontal->setSpacing(1);

	horizontal->addWidget(button = new QPushButton(tr("Add")));
	m_listener->connect(button, SIGNAL(clicked()), m_addSlot);

	horizontal->addWidget(button = new QPushButton(tr("Remove")));
	m_listener->connect(button, SIGNAL(clicked()), m_removeSlot);

	vertical->setMargin(1);
	vertical->setSpacing(1);
	vertical->addLayout(horizontal);
	vertical->addWidget(m_view = new QTreeView());
	m_view->setHeaderHidden(true);
	m_view->setItemDelegate(new ListConfigOptionDelegate(m_view));
	m_view->setModel(m_model = new ListConfigOptionModel(defaultValue(), m_view));

	return widget.take();
}

QVariant ListConfigOption::editorValue() const
{
	return m_model->value();
}

void ListConfigOption::setEditorValue(const QVariant &value)
{
	m_model->setValue(value);
}

void ListConfigOption::setLoadedEditorValue(const QVariant &value)
{
	m_model->setValue(value);
}

void ListConfigOption::setListener(QObject *listener, const char *addSlot, const char *removeSlot)
{
	m_listener = listener;
	m_addSlot = addSlot;
	m_removeSlot = removeSlot;
}

void ListConfigOption::add(const QString &value)
{
	m_model->add(value);
}

void ListConfigOption::remove()
{
	QModelIndex index = m_view->selectionModel()->currentIndex();

	if (index.isValid())
		m_model->remove(index);
}
