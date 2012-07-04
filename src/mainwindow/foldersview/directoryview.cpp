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
	m_eventHandler.registerShortcut(Qt::ALT,            Qt::Key_Left,      &DirectoryView::goBackward);
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
	m_eventHandler.registerShortcut(Qt::CTRL,           Qt::Key_F,         &DirectoryView::search);

	connect(&m_toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}

DirectoryView::~DirectoryView()
{
	if (m_node)
		m_node->viewClosed(this);
}

void DirectoryView::setupModel(const QString &absoluteFilePath)
{
	if (::History::Entry *entry = Application::rootNode()->open(this, absoluteFilePath))
		m_navigation.init(entry);
	else
		m_navigation.init(Application::rootNode()->open(this, defaultPath()));
}

void DirectoryView::setupModel(FileSystem::INode *node, const QModelIndex &index, const Geometry &geometry)
{
	if (::History::Entry *entry = node->viewInNewTab(this, index))
		m_navigation.init(entry);
	else
		m_navigation.init(Application::rootNode()->open(this, defaultPath()));
}

void DirectoryView::setupModel(const QString &path, qint32 column, Qt::SortOrder order, const Geometry &geometry)
{
	if (::History::Entry *entry = Application::rootNode()->open(this, path))
		m_navigation.init(entry);
	else
		m_navigation.init(Application::rootNode()->open(this, defaultPath()));
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
	m_view.setFocus();
	m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
	m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void DirectoryView::setNode(FileSystem::INode *node)
{
    m_node = node;
	m_view.setModel(node->model());
	m_view.setItemDelegate(node->delegate());
	m_view.setSelectionMode(node->selectionMode());
	m_header.pathEdit.setText(m_node->location());
	m_parent->updateTitle(this, m_node->title());

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

	Geometry geometry(m_node->geometry());

	for (Geometry::size_type i = 0, size = geometry.size(); i < size; ++i)
		m_view.setColumnWidth(i, geometry.at(i));

	::FileSystem::INode::Sorting sorting(m_node->sorting());
	m_view.sortByColumn(sorting.first, sorting.second);
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

DirectoryView::Tab DirectoryView::tab() const
{
	Geometry geometry;
	geometry.reserve(m_node->columnsCount());

	for (qint32 i = 0, size = m_node->columnsCount(); i < size; ++i)
		geometry.push_back(m_view.columnWidth(i));

	DirectoryView::Tab res =
	{m_node->location(), {m_view.header()->sortIndicatorSection(), m_view.header()->sortIndicatorOrder()}, geometry};

	return res;
}

void DirectoryView::setFocus()
{
	m_view.setFocus();
}

void DirectoryView::setCurrentDirectory(const QString &filePath)
{
	if (::History::Entry *entry = Application::rootNode()->open(this, filePath))
		m_navigation.save(entry);
}

void DirectoryView::goUp()
{
	if (::History::Entry *entry = m_node->viewParent(this))
		m_navigation.save(entry);
}

void DirectoryView::goBackward()
{
	if (History::Entry *entry = m_navigation.backward())
		m_node->viewHistory(this, entry);
}

void DirectoryView::goForward()
{
	if (History::Entry *entry = m_navigation.forward())
		m_node->viewHistory(this, entry);
}

void DirectoryView::refresh()
{
	m_node->refresh();
}

void DirectoryView::activated()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		if (::History::Entry *entry = m_node->viewChild(this, index))
			m_navigation.save(entry);
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

void DirectoryView::search()
{
	if (::History::Entry *entry = m_node->search(currentIndex(), this))
		m_navigation.save(entry);
}

void DirectoryView::actionTriggered(QAction *action)
{
	if (::History::Entry *entry = m_node->menuAction(action, this))
		m_navigation.save(entry);
}

void DirectoryView::openInNewTab()
{
	m_parent->openInNewTab(m_node, currentIndex(), geometry());
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
