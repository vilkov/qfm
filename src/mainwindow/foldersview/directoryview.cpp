#include "directoryview.h"
#include "foldersview.h"
#include "../../de/contextmenu/decontextmenu.h"
#include "../../tools/widgets/stringdialog/stringdialog.h"
#include "../../application.h"
#include <QtGui/QHeaderView>
#include <QtGui/QMessageBox>


DirectoryView::DirectoryView(FoldersView *parent) :
	QWidget(),
	m_parent(parent),
    m_node(0),
	m_layout(this),
    m_pathEventHandler(this),
    m_header(&m_pathEventHandler, this),
    m_toolBar(this),
	m_view(&m_eventHandler, this),
	m_eventHandler(this)
{
	m_layout.setMargin(3);
	m_layout.setSpacing(1);
	m_layout.addLayout(&m_header.layout);
	m_layout.addWidget(&m_toolBar);
	m_layout.addWidget(&m_view);

	m_view.setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_view.setSortingEnabled(true);
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
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F8,        &DirectoryView::createFile);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F7,        &DirectoryView::createDirectory);
	m_eventHandler.registerShortcut(Qt::SHIFT,          Qt::Key_Delete,    &DirectoryView::remove);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_Space,     &DirectoryView::calculateSize);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F5,        &DirectoryView::copy);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F6,        &DirectoryView::move);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_Escape,    &DirectoryView::cancel);

	connect(&m_toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}

DirectoryView::~DirectoryView()
{
	if (m_node)
		m_node->viewClosed(this);
}

void DirectoryView::setupModel(FileSystem::INode *root, const Tab &tab)
{
	root->viewAbsolute(this, tab.path, Application::plugins());

	for (Geometry::size_type i = 0, size = qMin(m_node->columnsCount(), tab.geometry.size()); i < size; ++i)
		m_view.setColumnWidth(i, tab.geometry.at(i));

	m_view.sortByColumn(tab.sort.column, tab.sort.order);
}

void DirectoryView::setupModel(FileSystem::INode *root, const QString &absoluteFilePath)
{
	root->viewAbsolute(this, absoluteFilePath, Application::plugins());
	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), Qt::AscendingOrder);
}

void DirectoryView::setupModel(FileSystem::INode *root, const QString &absoluteFilePath, const Geometry &geometry)
{
	root->viewAbsolute(this, absoluteFilePath, Application::plugins());

	for (Geometry::size_type i = 0, size = qMin(m_node->columnsCount(), geometry.size()); i < size; ++i)
		m_view.setColumnWidth(i, geometry.at(i));

	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), Qt::AscendingOrder);
}

FileSystem::INode *DirectoryView::node() const
{
	return m_node;
}

QModelIndex DirectoryView::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

QModelIndexList DirectoryView::selectedIndexes() const
{
	return m_view.selectionModel()->selectedIndexes();
}

void DirectoryView::edit(const QModelIndex &index)
{
	m_view.edit(index);
}

void DirectoryView::select(const QModelIndex &index)
{
	m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
	m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
	m_view.setFocus();
}

void DirectoryView::setNode(FileSystem::INode *node)
{
    m_node = node;
	m_view.setModel(node->model());
	m_view.setItemDelegate(node->delegate());
	m_view.setSelectionMode(node->selectionMode());
	m_header.pathEdit.setText(m_node->absoluteFilePath());
	m_parent->updateTitle(this, m_node->fileName());

	if (node->actions().isEmpty())
	{
		m_toolBar.setVisible(false);
		m_toolBar.clear();
	}
	else
	{
		m_toolBar.addActions(node->actions());
		m_toolBar.setVisible(true);
	}
}

QString DirectoryView::defaultPath()
{
#ifdef Q_OS_WIN
	return QString::fromLatin1("C:\\");
#else
	return QString::fromLatin1("/");
#endif
}

bool DirectoryView::hasFocus() const
{
	return m_header.pathEdit.hasFocus() || m_view.hasFocus();
}

QPoint DirectoryView::listPos() const
{
	return mapToGlobal(m_view.pos());
}

QString DirectoryView::currentDirectoryName() const
{
	return m_node->fileName();
}

void DirectoryView::save(QXmlStreamWriter &stream) const
{
	stream.writeTextElement(QString::fromLatin1("Path"), m_node->absoluteFilePath());

	stream.writeStartElement(QString::fromLatin1("Sort"));
	stream.writeTextElement(QString::fromLatin1("Column"), QString::number(m_view.header()->sortIndicatorSection()));
	stream.writeTextElement(QString::fromLatin1("Order"), QString::number(m_view.header()->sortIndicatorOrder()));
	stream.writeEndElement();

	QString name = QString::fromLatin1("Column");
	stream.writeStartElement(QString::fromLatin1("Geometry"));
	for (qint32 i = 0, size = m_node->columnsCount(); i < size; ++i)
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
	m_node->viewAbsolute(this, filePath, Application::plugins());
}

void DirectoryView::goUp()
{
//	m_navigation.insert(new History::DirectoryEntry(m_node->absolutePath()));
	m_node->viewParent(this);
	m_parent->updateTitle(this, m_node->fileName());
}

void DirectoryView::goBack()
{
//	if (History::Entry *entry = m_navigation.backward())
//		m_node->viewAbsolute(this, static_cast<History::DirectoryEntry *>(entry)->direcory(), Application::plugins());
}

void DirectoryView::goForward()
{
//	if (History::Entry *entry = m_navigation.forvard())
//		m_node->viewAbsolute(this, static_cast<History::DirectoryEntry *>(entry)->direcory(), Application::plugins());
}

void DirectoryView::refresh()
{
	m_node->refresh();
}

void DirectoryView::activated()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
	{
//		m_navigation.insert(new History::DirectoryEntry(m_node->info(index)->absoluteFilePath()));
		m_node->viewChild(this, index, Application::plugins());
	}
}

void DirectoryView::pathToClipboard()
{
	m_node->pathToClipboard(selectedIndexes(), this);
}

void DirectoryView::rename()
{
	m_node->rename(currentIndex(), this);
}

void DirectoryView::createFile()
{
	m_node->createFile(currentIndex(), this);
}

void DirectoryView::createDirectory()
{
	m_node->createDirectory(currentIndex(), this);
}

void DirectoryView::remove()
{
	m_node->remove(selectedIndexes(), this);
}

void DirectoryView::calculateSize()
{
	m_node->calculateSize(selectedIndexes(), this);
}

void DirectoryView::copy()
{
	m_node->copy(this, static_cast<DirectoryView*>(m_parent->other().currentWidget()));
}

void DirectoryView::move()
{
	m_node->move(this, static_cast<DirectoryView*>(m_parent->other().currentWidget()));
}

void DirectoryView::cancel()
{
	m_node->cancel(selectedIndexes(), this);
}

void DirectoryView::actionTriggered(QAction *action)
{
	m_node->menuAction(action, this);
}

void DirectoryView::openInNewTab()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		m_parent->openInNewTab(m_node, m_node->info(index)->absoluteFilePath(), geometry());
}

void DirectoryView::closeTab()
{
	m_parent->closeCurrentTab();
}

void DirectoryView::editPath()
{
	if (m_header.pathEditFlag)
	{
		m_header.pathEditFlag = false;
		m_header.pathEdit.startEdit();
	}
}

void DirectoryView::contextMenu()
{
	m_node->contextMenu(selectedIndexes(), this);
}

void DirectoryView::refreshOther()
{
	static_cast<DirectoryView*>(m_parent->other().currentWidget())->refresh();
}

QList<qint32> DirectoryView::geometry() const
{
	QList<qint32> res;
	res.reserve(m_node->columnsCount());

	for (qint32 i = 0, size = m_node->columnsCount(); i < size; ++i)
		res.push_back(m_view.columnWidth(i));

	return res;
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
	setCurrentDirectory(m_header.pathEdit.text().trimmed());
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
