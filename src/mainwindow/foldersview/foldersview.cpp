#include "foldersview.h"
#include "../../tools/pointers/pscopedpointer.h"


FoldersView::FoldersView(const QString &id, Tools::Settings::Scope *settings, FoldersViewRef other, QWidget *parent) :
	QWidget(parent),
    m_settings(id, settings, this, &FoldersView::save, &FoldersView::load),
	m_doNotRefreshTab(true),
    m_layout(this),
    m_tabWidget(this),
	m_other(other)
{
	m_layout.setMargin(3);
	m_layout.setSpacing(1);
	m_layout.addWidget(&m_tabWidget);
	m_layout.setStretchFactor(&m_tabWidget, 1);

	connect(&m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshTab(int)));
}

void FoldersView::refresh()
{
	doRefresh(m_tabWidget.currentWidget());
}

void FoldersView::updateTitle(QWidget *widget, const QString &fileName)
{
	m_tabWidget.setTabText(m_tabWidget.indexOf(widget), fileName);
}

void FoldersView::openInNewTab(::VFS::INode *node, const QModelIndex &index, const QList<qint32> &geometry)
{
	DirectoryView *widget;
	m_doNotRefreshTab = true;
	m_tabWidget.setCurrentIndex(m_tabWidget.addTab(widget = new DirectoryView(this), QString()));
	widget->setupModel(node, index, geometry);
	widget->refresh();
	widget->setFocus();
}

void FoldersView::closeCurrentTab()
{
	if (m_tabWidget.count() > 1)
	{
		PScopedPointer<QWidget> widget(m_tabWidget.currentWidget());
		m_tabWidget.removeTab(m_tabWidget.currentIndex());
		static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setFocus();
	}
}

void FoldersView::setFocus()
{
	static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setFocus();
}

void FoldersView::setCurrentDirectory(const QString &filePath)
{
	static_cast<DirectoryView *>(m_tabWidget.currentWidget())->setCurrentDirectory(filePath);
}

void FoldersView::skipOneRefreshTab()
{
	m_doNotRefreshTab = true;
}

void FoldersView::refreshTab(int index)
{
	doRefresh(m_tabWidget.widget(index));
}

void FoldersView::doRefresh(QWidget *tab)
{
	if (m_doNotRefreshTab)
		m_doNotRefreshTab = false;
	else
		static_cast<DirectoryView*>(tab)->refresh();
}

void FoldersView::save()
{
	m_settings.setActiveTab(m_tabWidget.currentIndex());

	for (qint32 i = 0, size = m_tabWidget.count(); i < size; ++i)
		m_settings.tabs().add(static_cast<DirectoryView *>(m_tabWidget.widget(i))->tab());
}

void FoldersView::load()
{
	if (m_settings.tabs().isEmpty())
	{
		DirectoryView *widget;
		m_tabWidget.addTab(widget = new DirectoryView(this), QString());
		widget->setupModel(DirectoryView::defaultPath());
	}
	else
	{
		Tab *tab;
		DirectoryView *widget;

		for (Tabs::size_type i = 0, size = m_settings.tabs().size(); i < size; ++i)
		{
			tab = m_settings.tabs().at(i)->as<Tab>();
			m_tabWidget.addTab(widget = new DirectoryView(this), QString());
			widget->setupModel(tab->path(), tab->sort().column(), tab->sort().order(), tab->geometry());
		}

		m_tabWidget.setCurrentIndex(m_settings.activeTab());
		m_settings.tabs().clear();
	}
}

FoldersView::Sort::Sort(Option *parent) :
	Scope(QString::fromLatin1("Sort"), parent),
	m_column(QString::fromLatin1("Column"), this),
	m_order(QString::fromLatin1("Order"), this)
{
	manage(&m_column);
	manage(&m_order);
}

FoldersView::Sort::Sort(const DirectoryView::Tab::Sort &sort, Option *parent) :
	Scope(QString::fromLatin1("Sort"), parent),
	m_column(QString::fromLatin1("Column"), this),
	m_order(QString::fromLatin1("Order"), this)
{
	manage(&m_column);
	manage(&m_order);

	m_column.setValue(QString::number(sort.column));
	m_order.setValue(QString::number(sort.order));
}

FoldersView::Tab::Tab(Option *parent) :
	Scope(QString::fromLatin1("Tab"), parent),
	m_path(QString::fromLatin1("Path"), this),
	m_sort(this),
	m_geometry(QString::fromLatin1("Geometry"), this)
{
	manage(&m_path);
	manage(&m_sort);
	manage(&m_geometry);
}

FoldersView::Tab::Tab(const DirectoryView::Tab &tab, Option *parent) :
	Scope(QString::fromLatin1("Tab"), parent),
	m_path(QString::fromLatin1("Path"), this),
	m_sort(tab.sort, this),
	m_geometry(QString::fromLatin1("Geometry"), this)
{
	manage(&m_path);
	manage(&m_sort);
	manage(&m_geometry);

	m_path.setValue(tab.path);

	for (DirectoryView::Geometry::size_type i = 0, size = tab.geometry.size(); i < size; ++i)
		m_geometry.add(QString::number(tab.geometry.at(i)));
}

::VFS::INode::Geometry FoldersView::Tab::geometry() const
{
	::VFS::INode::Geometry res;
	res.reserve(m_geometry.size());

	for (Tools::Settings::OptionValueList::size_type i = 0, size = m_geometry.size(); i < size; ++i)
		res.push_back(m_geometry.at(i).toInt());

	return res;
}

FoldersView::Tabs::Tabs(Option *parent) :
	OptionList(QString::fromLatin1("Tabs"), parent)
{}

void FoldersView::Tabs::add(const DirectoryView::Tab &tab)
{
	OptionList::add(new Tab(tab, this));
}

bool FoldersView::Tabs::isSubOptionName(const QStringRef &name) const
{
	static const QString optionName = QString::fromLatin1("Tab");
	return name == optionName;
}

Tools::Settings::Option *FoldersView::Tabs::create()
{
	return new Tab(this);
}

FoldersView::Settings::Settings(const QString &id, Scope *parent, FoldersView *object, Method save, Method load) :
	Scope(id, parent),
	m_activeTab(QString::fromLatin1("ActiveTab"), this),
	m_tabs(this),
	m_object(object),
	m_save(save),
	m_load(load)
{
	parent->manage(this);
	manage(&m_activeTab);
	manage(&m_tabs);
}

void FoldersView::Settings::save(QXmlStreamWriter &stream) const
{
	(m_object->*m_save)();
	Scope::save(stream);
}

void FoldersView::Settings::load(QXmlStreamReader &stream)
{
	Scope::load(stream);
	(m_object->*m_load)();
}

void FoldersView::Settings::loadDefault()
{
	Scope::loadDefault();
	(m_object->*m_load)();
}
