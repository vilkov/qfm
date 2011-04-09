#include "foldersview.h"
#include "directoryview.h"
#include <QScopedPointer>


FoldersView::FoldersView(const QStringList &folders, QWidget *parent) :
	QWidget(parent),
    m_layout(this),
    m_tabWidget(this)
{
	setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.setSpacing(1);
	m_layout.addWidget(&m_tabWidget);
	m_layout.setStretchFactor(&m_tabWidget, 1);

	for (QStringList::size_type i = 0, size = folders.size(); i < size; ++i)
		m_tabWidget.addTab(new DirectoryView(folders.at(i), this), QString());

	updateCurrentDirectory(static_cast<DirectoryView*>(m_tabWidget.currentWidget())->currentDirectoryInfo());
}

void FoldersView::updateCurrentDirectory(const QFileInfo &info)
{
	QString fileName = info.fileName();

	if (fileName.isEmpty())
		m_tabWidget.setTabText(m_tabWidget.currentIndex(), QString::fromLatin1("/"));
	else
		m_tabWidget.setTabText(m_tabWidget.currentIndex(), fileName);
}

void FoldersView::openInNewTab(const QFileInfo &fileInfo)
{
	m_tabWidget.setCurrentIndex(m_tabWidget.addTab(new DirectoryView(fileInfo, this), QString()));
	updateCurrentDirectory(static_cast<DirectoryView*>(m_tabWidget.currentWidget())->currentDirectoryInfo());
	static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
}

void FoldersView::closeCurrentTab()
{
	if (m_tabWidget.count() > 1)
	{
		QScopedPointer<QWidget> widget(m_tabWidget.currentWidget());
		m_tabWidget.removeTab(m_tabWidget.currentIndex());
		updateCurrentDirectory(static_cast<DirectoryView*>(m_tabWidget.currentWidget())->currentDirectoryInfo());
		static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
	}
}

void FoldersView::setFocus()
{
	static_cast<DirectoryView*>(m_tabWidget.currentWidget())->setFocus();
}
