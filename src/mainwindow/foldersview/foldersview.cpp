#include "foldersview.h"
#include "../../tools/pointers/pscopedpointer.h"


FoldersView::FoldersView(FileSystem::INode *root, const TabList &tabs, FoldersViewRef other, QWidget *parent) :
	QWidget(parent),
	m_doNotRefreshTab(true),
    m_layout(this),
    m_tabWidget(this),
	m_other(other)
{
	Q_ASSERT(root != 0);

	m_layout.setMargin(3);
	m_layout.setSpacing(1);
	m_layout.addWidget(&m_tabWidget);
	m_layout.setStretchFactor(&m_tabWidget, 1);

	if (tabs.isEmpty())
	{
		DirectoryView *widget;
		m_tabWidget.addTab(widget = new DirectoryView(this), QString());
		widget->setupModel(root, DirectoryView::defaultPath());
	}
	else
	{
		qint32 activeWidget = 0;
		DirectoryView *widget;

		for (TabList::size_type i = 0, size = tabs.size(); i < size; ++i)
		{
			const TabList::value_type &tab = tabs.at(i);

			if (tab.isActive)
				activeWidget = i;

			m_tabWidget.addTab(widget = new DirectoryView(this), QString());
			widget->setupModel(root, tab.tab);
		}

		m_tabWidget.setCurrentIndex(activeWidget);
	}

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

void FoldersView::openInNewTab(FileSystem::INode *root, const QString &absoluteFilePath, const QList<qint32> &geometry)
{
	DirectoryView *widget;
	m_doNotRefreshTab = true;
	m_tabWidget.setCurrentIndex(m_tabWidget.addTab(widget = new DirectoryView(this), QString()));
	widget->setupModel(root, absoluteFilePath, geometry);
	widget->refresh();
	widget->setFocus();
}

void FoldersView::closeCurrentTab()
{
	if (m_tabWidget.count() > 1)
	{
		PScopedPointer<QWidget> widget(m_tabWidget.currentWidget());
		m_tabWidget.removeTab(m_tabWidget.currentIndex());
		static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
	}
}

void FoldersView::setFocus()
{
	static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
}

void FoldersView::setCurrentDirectory(const QString &filePath)
{
	static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setCurrentDirectory(filePath);
}

void FoldersView::skipOneRefreshTab()
{
	m_doNotRefreshTab = true;
}

void FoldersView::saveTabs(QXmlStreamWriter &stream) const
{
	QString name = QString::fromLatin1("Tab");

	stream.writeTextElement(QString::fromLatin1("ActiveTab"), QString::number(m_tabWidget.currentIndex()));

	for (qint32 i = 0, size = m_tabWidget.count(); i < size; ++i)
	{
		stream.writeStartElement(name + QString::number(i));
		static_cast<DirectoryView*>(m_tabWidget.widget(i))->save(stream);
		stream.writeEndElement();
	}
}

FoldersView::TabList FoldersView::loadTabs(QXmlStreamReader &stream)
{
	qint32 activeTab = 0;
	QString name;
	QString tabName = QString::fromLatin1("Tab");
	TabList res;

	if (stream.readNextStartElement() && stream.name() == QString::fromLatin1("ActiveTab"))
		activeTab = stream.readElementText().toInt();

	while (stream.readNextStartElement())
		if ((name = stream.name().toString()).startsWith(tabName))
			res.push_back(Tab(DirectoryView::load(stream, name)));

	if (activeTab < res.size())
		res[activeTab].isActive = true;

	return res;
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
