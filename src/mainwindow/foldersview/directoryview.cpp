#include "directoryview.h"
#include "foldersview.h"
#include "../../tools/widgets/stringdialog/stringdialog.h"
#include <QtGui/QHeaderView>
#include <QtGui/QMessageBox>


DirectoryView::DirectoryView(const Tab &tab, FoldersView *parent) :
	QWidget(),
	m_parent(parent),
	m_menu(this),
    m_layout(this),
    m_pathEventHandler(this),
    m_header(&m_pathEventHandler, this),
    m_view(&m_eventHandler, this),
    m_model(tab.path, this),
    m_delegate(&m_proxy),
	m_eventHandler(this)
{
	Q_ASSERT(m_model.columnCount() == tab.geometry.size());

	initialize();

	for (qint32 i = 0, size = m_model.columnCount(); i < size; ++i)
		m_view.setColumnWidth(i, tab.geometry.at(i));

	m_view.sortByColumn(tab.sort.column, tab.sort.order);
}

DirectoryView::DirectoryView(const FileSystemInfo &fileInfo, FoldersView *parent) :
	QWidget(),
	m_parent(parent),
	m_menu(this),
	m_layout(this),
    m_pathEventHandler(this),
    m_header(&m_pathEventHandler, this),
	m_view(&m_eventHandler, this),
	m_model(fileInfo, this),
    m_delegate(&m_proxy),
	m_eventHandler(this)
{
	initialize();
}

DirectoryView::DirectoryView(const FileSystemInfo &fileInfo, const QList<qint32> &geometry, FoldersView *parent) :
	QWidget(),
	m_parent(parent),
	m_menu(this),
	m_layout(this),
	m_pathEventHandler(this),
	m_header(&m_pathEventHandler, this),
	m_view(&m_eventHandler, this),
	m_model(fileInfo, this),
	m_delegate(&m_proxy),
	m_eventHandler(this)
{
	Q_ASSERT(m_model.columnCount() == geometry.size());

	initialize();

	for (qint32 i = 0, size = m_model.columnCount(); i < size; ++i)
		m_view.setColumnWidth(i, geometry.at(i));
}

const FileSystemInfo &DirectoryView::currentDirectoryInfo() const
{
	return m_model.currentDirectoryInfo();
}

void DirectoryView::save(QXmlStreamWriter &stream) const
{
	stream.writeTextElement(QString::fromLatin1("Path"), m_model.currentDirectoryInfo().absoluteFilePath());

	stream.writeStartElement(QString::fromLatin1("Sort"));
	stream.writeTextElement(QString::fromLatin1("Column"), QString::number(m_view.header()->sortIndicatorSection()));
	stream.writeTextElement(QString::fromLatin1("Order"), QString::number(m_view.header()->sortIndicatorOrder()));
	stream.writeEndElement();

	QString name = QString::fromLatin1("Column");
	stream.writeStartElement(QString::fromLatin1("Geometry"));
	for (qint32 i = 0, size = m_model.columnCount(); i < size; ++i)
		stream.writeTextElement(name + QString::number(i), QString::number(m_view.columnWidth(i)));
	stream.writeEndElement();
}

DirectoryView::Tab DirectoryView::load(QXmlStreamReader &stream, const QString &stopTagName)
{
	DirectoryView::Tab res;
	QString column = QString::fromLatin1("Column");

	if (stream.readNextStartElement() && stream.name() == QString::fromLatin1("Path"))
		res.path = stream.readElementText();

	if (stream.readNextStartElement() && stream.name() == QString::fromLatin1("Sort"))
	{
		if (stream.readNextStartElement() && stream.name() == column)
			res.sort.column = stream.readElementText().toInt();

		if (stream.readNextStartElement() && stream.name() == QString::fromLatin1("Order"))
			res.sort.order = static_cast<Qt::SortOrder>(stream.readElementText().toInt());

		stream.readNextStartElement();
	}

	if (stream.readNextStartElement() && stream.name() == QString::fromLatin1("Geometry"))
	{
		while (stream.readNextStartElement() && stream.name().toString().startsWith(column))
			res.geometry.push_back(stream.readElementText().toInt());

		stream.readNextStartElement();
	}

	return res;
}

void DirectoryView::setFocus()
{
	m_view.setFocus();
}

void DirectoryView::setCurrentDirectory(const QString &filePath)
{
	FileSystemInfo info(filePath);

	if (info.exists())
	{
		QModelIndex index = m_model.setCurrentDirectory(info);

		if (index.isValid())
			selectIndex(toViewIndex(index));
	}

	updateCurrentDirectory(m_model.currentDirectoryInfo());
}

void DirectoryView::goUp()
{
	QModelIndex index = m_model.rootIndex();

	if (index.isValid())
	{
		QModelIndex parentEntryIndex = m_model.parentEntryIndex();

		m_model.activated(index);
		updateCurrentDirectory(m_model.currentDirectoryInfo());

		if (parentEntryIndex.isValid())
			selectIndex(toViewIndex(parentEntryIndex));
	}
}

void DirectoryView::goBack()
{

}

void DirectoryView::goForward()
{

}

void DirectoryView::refresh()
{
	m_model.refresh();
	updateCurrentDirectory(m_model.currentDirectoryInfo());
}

void DirectoryView::activated()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
	{
		QModelIndex index2 = m_model.rootIndex();

		if (index == index2 && (index2 = m_model.parentEntryIndex()).isValid())
		{
			m_model.activated(index);
			updateCurrentDirectory(m_model.currentDirectoryInfo());
			selectIndex(toViewIndex(index2));
		}
		else
			if (m_model.fileInfo(index).isDir())
			{
				m_model.activated(index);
				updateCurrentDirectory(m_model.currentDirectoryInfo());
				selectIndex(toViewIndex(m_model.rootIndex()));
			}
			else
				m_model.activated(index);
	}
}

void DirectoryView::pathToClipboard()
{
	m_model.pathToClipboard(selectedIndexes());
}

void DirectoryView::rename()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
	{
		const FileSystemInfo &info = m_model.fileInfo(index);

		if (info.exists())
		{
			QString fileName = info.fileName();

			StringDialog dialog(
					info.isDir() ?
							tr("Enter new name for directory \"%1\"").arg(fileName) :
							tr("Enter new name for file \"%1\"").arg(fileName),
					tr("Name"),
					fileName,
					this);

			if (dialog.exec() == QDialog::Accepted)
				m_model.rename(index, dialog.value());
		}
	}
}

void DirectoryView::createDirectory()
{
	StringDialog dialog(
			tr("Enter name for directory"),
			tr("Name"),
			QString(),
			this);

	if (dialog.exec() == QDialog::Accepted)
		m_model.createDirectory(dialog.value());
}

void DirectoryView::remove()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		m_model.remove(index);
}

void DirectoryView::calculateSize()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		m_model.refreshSize(index);
}

void DirectoryView::copy()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		m_model.copy(index, &static_cast<DirectoryView*>(m_parent->other().currentWidget())->m_model);
}

void DirectoryView::move()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		m_model.move(index, &static_cast<DirectoryView*>(m_parent->other().currentWidget())->m_model);
}

void DirectoryView::openInNewTab()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
	{
		const FileSystemInfo &info = m_model.fileInfo(index);

		if (info.isDir())
			m_parent->openInNewTab(info, geometry());
	}
}

void DirectoryView::closeTab()
{
	QStringList list = m_model.lockedEntries();

	if (list.isEmpty())
		m_parent->closeCurrentTab();
	else
		QMessageBox::information(
				this,
				tr("Unfinished tasks..."),
				tr("There is a busy entries:\n").append(list.join(QChar('\n'))));
}

void DirectoryView::editPath()
{
	if (m_header.pathEditFlag)
	{
		m_header.pathEditFlag = false;
		m_header.pathEdit.startEdit();
	}
}

void DirectoryView::selectIndex(const QModelIndex &index)
{
	m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
	m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
	m_view.setFocus();
}

void DirectoryView::updateCurrentDirectory(const FileSystemInfo &info)
{
	m_header.pathEdit.setText(info.absoluteFilePath());
	m_parent->updateTitle(info);
}

void DirectoryView::contextMenu()
{
	QStringList selectedFiles;
	m_menu.popup(m_model.selectedFiles(selectedIndexes(), selectedFiles), selectedFiles);
}

void DirectoryView::refreshOther()
{
	static_cast<DirectoryView*>(m_parent->other().currentWidget())->refresh();
}

QList<qint32> DirectoryView::geometry() const
{
	QList<qint32> res;
	res.reserve(m_model.columnCount());

	for (qint32 i = 0, size = m_model.columnCount(); i < size; ++i)
		res.push_back(m_view.columnWidth(i));

	return res;
}

void DirectoryView::initialize()
{
	setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.addLayout(&m_header.layout);
	m_layout.addWidget(&m_view);

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(&m_model);
	m_view.setModel(&m_proxy);
	m_view.setSelectionMode(QAbstractItemView::SingleSelection);
	m_view.setItemDelegate(&m_delegate);
	m_view.setSortingEnabled(true);
//	m_view.setUniformRowHeights(true);
	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), Qt::AscendingOrder);
	m_view.setContextMenuPolicy(Qt::DefaultContextMenu);

	m_eventHandler.registerContextMenuEventHandler(&DirectoryView::contextMenu);
	m_eventHandler.registerMouseDoubleClickEventHandler(&DirectoryView::activated);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_Backspace, &DirectoryView::goUp);
	m_eventHandler.registerShortcut(Qt::ALT,            Qt::Key_Left,      &DirectoryView::goBack);
	m_eventHandler.registerShortcut(Qt::ALT,            Qt::Key_Right,     &DirectoryView::goForward);
	m_eventHandler.registerShortcut(Qt::ALT,            Qt::Key_Up,        &DirectoryView::editPath);
	m_eventHandler.registerShortcut(Qt::CTRL,           Qt::Key_F5,        &DirectoryView::refresh);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_Return,    &DirectoryView::activated);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_Enter,     &DirectoryView::activated);
	m_eventHandler.registerShortcut(Qt::ALT + Qt::CTRL, Qt::Key_X,         &DirectoryView::pathToClipboard);
	m_eventHandler.registerShortcut(Qt::CTRL,           Qt::Key_Return,    &DirectoryView::openInNewTab);
	m_eventHandler.registerShortcut(Qt::CTRL,           Qt::Key_W,         &DirectoryView::closeTab);
	m_eventHandler.registerShortcut(Qt::CTRL,           Qt::Key_Down,      &DirectoryView::closeTab);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F2,        &DirectoryView::rename);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F7,        &DirectoryView::createDirectory);
	m_eventHandler.registerShortcut(Qt::SHIFT,          Qt::Key_Delete,    &DirectoryView::remove);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_Space,     &DirectoryView::calculateSize);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F5,        &DirectoryView::copy);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F6,        &DirectoryView::move);

	m_header.pathEdit.setText(m_model.currentDirectoryInfo().absoluteFilePath());
}

QModelIndex DirectoryView::currentIndex() const
{
	return m_proxy.mapToSource(m_view.selectionModel()->currentIndex());
}

QModelIndexList DirectoryView::selectedIndexes() const
{
	QModelIndexList res;
	QModelIndexList list = m_view.selectionModel()->selectedIndexes();

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		res.push_back(m_proxy.mapToSource(list.at(i)));

	return res;
}

QModelIndex DirectoryView::toViewIndex(const QModelIndex &index) const
{
	return m_proxy.mapFromSource(index);
}

DirectoryView::Header::Header(PathEventHandler *eventHandler, QWidget *parent) :
	pathEdit(eventHandler, parent),
	pathEditFlag(true),
	favorites(parent),
	history(parent)
{
	layout.setMargin(1);
	layout.setSpacing(1);
	layout.addWidget(&pathEdit);
	layout.setStretchFactor(&pathEdit, 1);
//	path.setFrameStyle(QFrame::Sunken | QFrame::Panel);
	eventHandler->registerMouseReleaseEventHandler(&DirectoryView::editPath);
	eventHandler->registerFocusOutEventHandler(&DirectoryView::actHeaderPathReject);
	eventHandler->registerShortcut(Qt::NoModifier, Qt::Key_Return, &DirectoryView::actHeaderPathAccept);
	eventHandler->registerShortcut(Qt::NoModifier, Qt::Key_Escape, &DirectoryView::actHeaderPathReject);

	favorites.setText(QString::fromLatin1("*"));
	layout.addWidget(&favorites);
	parent->connect(&favorites, SIGNAL(clicked()), SLOT(actHeaderFavorites()));

	history.setText(QString::fromLatin1("H"));
	layout.addWidget(&history);
	parent->connect(&history, SIGNAL(clicked()), SLOT(actHeaderHistory()));
}

void DirectoryView::actHeaderPathAccept()
{
	m_header.pathEditFlag = true;
	setCurrentDirectory(m_header.pathEdit.text());
	setFocus();
}

void DirectoryView::actHeaderPathReject()
{
	if (!m_header.pathEditFlag)
	{
		m_header.pathEditFlag = true;
		m_header.pathEdit.cancelEdit();
		setFocus();
	}
}

void DirectoryView::actHeaderFavorites()
{
	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), m_view.header()->sortIndicatorOrder());
}

void DirectoryView::actHeaderHistory()
{

}
