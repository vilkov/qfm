#include "directoryview.h"
#include "foldersview.h"

#include <vfs/interfaces/vfs_inode.h>
#include <desktop/contextmenu/desktop_contextmenu.h>
#include <tools/widgets/stringdialog/stringdialog.h>
#include <tools/optimizations/optimizations.h>
#include <application.h>

#include <QtGui/QHeaderView>
#include <QtGui/QMessageBox>


DirectoryView::DirectoryView(const QString &absoluteFilePath, FoldersView *parent) :
	BaseClass(),
	m_parent(parent),
    m_node(NULL),
	m_layout(this),
    m_eventHandler(this),
    m_pathEventHandler(this),
    m_header(&m_pathEventHandler, this),
    m_toolBar(this),
	m_view(eventHandler(), this)
{
	Q_ASSERT(m_parent);
	initialize();

	if (::History::Entry *entry = Application::rootNode()->open(this, absoluteFilePath))
		initializeHistory(entry);
	else
		initializeHistory(Application::rootNode()->open(this, defaultPath()));
}

DirectoryView::DirectoryView(::VFS::INode *node, const QModelIndex &index, const Geometry &geometry, FoldersView *parent) :
	BaseClass(),
	m_parent(parent),
	m_node(NULL),
	m_layout(this),
	m_eventHandler(this),
	m_pathEventHandler(this),
	m_header(&m_pathEventHandler, this),
	m_toolBar(this),
	m_view(eventHandler(), this)
{
	Q_ASSERT(m_parent);
	initialize();

	if (::History::Entry *entry = node->viewInNewTab(this, index))
		initializeHistory(entry);
	else
		initializeHistory(Application::rootNode()->open(this, defaultPath()));
}

DirectoryView::DirectoryView(const QString &path, qint32 column, Qt::SortOrder order, const Geometry &geometry, FoldersView *parent) :
	BaseClass(),
	m_parent(parent),
	m_node(NULL),
	m_layout(this),
	m_eventHandler(this),
	m_pathEventHandler(this),
	m_header(&m_pathEventHandler, this),
	m_toolBar(this),
	m_view(eventHandler(), this)
{
	Q_ASSERT(m_parent);
	initialize();

	if (::History::Entry *entry = Application::rootNode()->open(this, path))
		initializeHistory(entry);
	else
		initializeHistory(Application::rootNode()->open(this, defaultPath()));
}

DirectoryView::~DirectoryView()
{
	if (m_node)
		m_node->viewClosed(this);
}

::VFS::INode *DirectoryView::node() const
{
	return m_node;
}

::VFS::INodeView *DirectoryView::opposite() const
{
	return static_cast<DirectoryView *>(m_parent->other().currentWidget());
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

void DirectoryView::setNode(::VFS::INode *node)
{
	/**
	 * m_node == NULL
	 * only during construction of DirectoryView objects.
	 */
	if (LIKELY(m_node != NULL))
		m_parent->updateTitle(this, node->title());

    m_node = node;
	m_view.setModel(m_node->model());
	m_view.setItemDelegate(m_node->delegate());
	m_view.setSelectionMode(m_node->selectionMode());
	m_header.pathEdit.setText(m_node->location());

	if (m_node->actions().isEmpty())
	{
		m_toolBar.setVisible(false);
		m_toolBar.clear();
	}
	else
	{
		m_toolBar.addActions(m_node->actions());
		m_toolBar.setVisible(true);
	}

	Geometry geometry(m_node->geometry());

	for (Geometry::size_type i = 0, size = geometry.size(); i < size; ++i)
		m_view.setColumnWidth(i, geometry.at(i));

	::VFS::INode::Sorting sorting(m_node->sorting());
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

QString DirectoryView::title() const
{
	return m_node->title();
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
		save(entry);
}

void DirectoryView::goUp()
{
	BaseClass::goUp();
}

void DirectoryView::goBackward()
{
	BaseClass::goBackward();
}

void DirectoryView::goForward()
{
	BaseClass::goForward();
}

void DirectoryView::refresh()
{
	BaseClass::refresh();
}

void DirectoryView::cancel()
{
	BaseClass::cancel();
}

bool DirectoryView::handleShortcut(QEvent *event)
{
	return m_eventHandler.keyboardEvent(static_cast<QKeyEvent *>(event)) ? true : BaseClass::handleShortcut(event);
}

void DirectoryView::actionTriggered(QAction *action)
{
	m_node->menuAction(this, action);
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

void DirectoryView::initialize()
{
	m_layout.setMargin(3);
	m_layout.setSpacing(1);
	m_layout.addLayout(&m_header.layout);
	m_layout.addWidget(&m_toolBar);
	m_layout.addWidget(&m_view);

	m_view.setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_view.setSortingEnabled(true);
	m_view.setContextMenuPolicy(Qt::DefaultContextMenu);

	m_eventHandler.registerShortcut(Qt::ALT,  Qt::Key_Up,     &DirectoryView::editPath);
	m_eventHandler.registerShortcut(Qt::CTRL, Qt::Key_Return, &DirectoryView::openInNewTab);
	m_eventHandler.registerShortcut(Qt::CTRL, Qt::Key_W,      &DirectoryView::closeTab);
	m_eventHandler.registerShortcut(Qt::CTRL, Qt::Key_Down,   &DirectoryView::closeTab);

	connect(&m_toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}
