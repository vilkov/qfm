#include "foldersview.h"
#include <QtCore/QScopedPointer>


FoldersView::FoldersView(FileSystem::INode *root, const TabList &tabs, FoldersViewRef other, QWidget *parent) :
	QWidget(parent),
	m_doNotRefreshTab(true),
    m_layout(this),
    m_tabWidget(this),
	m_other(other)
{
	Q_ASSERT(root != 0);

	setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.setSpacing(1);
	m_layout.addWidget(&m_tabWidget);
	m_layout.setStretchFactor(&m_tabWidget, 1);

	if (tabs.isEmpty())
	{
		DirectoryView *widget;
		m_tabWidget.addTab(widget = new DirectoryView(root, this), QString());
		updateTitle(widget->currentDirectoryName());
		static_cast<DirectoryView*>(widget)->refresh();
	}
	else
	{
		qint32 index;
		qint32 activeWidget = 0;
		DirectoryView *widget;

		for (TabList::size_type i = 0, size = tabs.size(); i < size; ++i)
		{
			const TabList::value_type &tab = tabs.at(i);

			if (tab.isActive)
				activeWidget = i;

			index = m_tabWidget.addTab(widget = new DirectoryView(root, tab.tab, this), QString());
			updateTitle(index, widget->currentDirectoryName());
		}

		m_tabWidget.setCurrentIndex(activeWidget);
		static_cast<DirectoryView*>(widget)->refresh();
	}

	connect(&m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshTab(int)));
}

void FoldersView::refresh()
{
	doRefresh(m_tabWidget.currentWidget());
}

void FoldersView::updateTitle(const QString &fileName)
{
	updateTitle(m_tabWidget.currentIndex(), fileName);
}

void FoldersView::updateTitle(qint32 index, const QString &fileName)
{
	if (fileName.isEmpty())
		m_tabWidget.setTabText(index, DirectoryView::rootPath());
	else
		m_tabWidget.setTabText(index, fileName);
}

void FoldersView::openInNewTab(const FileSystem::Info &fileInfo, const QList<qint32> &geometry)
{
//	m_doNotRefreshTab = true;
//	m_tabWidget.setCurrentIndex(m_tabWidget.addTab(new DirectoryView(root, fileInfo, geometry, this), QString()));
//	updateTitle(static_cast<DirectoryView*>(m_tabWidget.currentWidget())->currentDirectoryName());
//	static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
}

void FoldersView::closeCurrentTab()
{
	if (m_tabWidget.count() > 1)
	{
		QScopedPointer<QWidget> widget(m_tabWidget.currentWidget());
		m_tabWidget.removeTab(m_tabWidget.currentIndex());
		updateTitle(static_cast<DirectoryView*>(m_tabWidget.currentWidget())->currentDirectoryName());
		static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
	}
}

void FoldersView::setFocus()
{
	static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
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
