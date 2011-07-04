#include "directoryview.h"
#include "foldersview.h"
#include "../../tools/widgets/stringdialog/stringdialog.h"
#include "../../application.h"
#include <QtGui/QHeaderView>
#include <QtGui/QMessageBox>


DirectoryView::DirectoryView(FoldersView *parent) :
	QWidget(),
	m_parent(parent),
    m_node(0),
	m_menu(this),
	m_layout(this),
    m_pathEventHandler(this),
    m_header(&m_pathEventHandler, this),
	m_view(&m_eventHandler, this),
	m_eventHandler(this)
{
	setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.addLayout(&m_header.layout);
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
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F7,        &DirectoryView::createDirectory);
	m_eventHandler.registerShortcut(Qt::SHIFT,          Qt::Key_Delete,    &DirectoryView::remove);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_Space,     &DirectoryView::calculateSize);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F5,        &DirectoryView::copy);
	m_eventHandler.registerShortcut(Qt::NoModifier,     Qt::Key_F6,        &DirectoryView::move);
}

DirectoryView::~DirectoryView()
{
	if (m_node)
		m_node->viewClosed(this);
}

void DirectoryView::setupModel(FileSystem::INode *root, const Tab &tab)
{
	root->viewChild(this, FileSystem::Path(tab.path).begin(), Application::instance()->mainWindow().plugins());

	for (Geometry::size_type i = 0, size = qMin(m_node->columnCount(), tab.geometry.size()); i < size; ++i)
		m_view.setColumnWidth(i, tab.geometry.at(i));

	m_view.sortByColumn(tab.sort.column, tab.sort.order);
}

void DirectoryView::setupModel(FileSystem::INode *root, const QString &absoluteFilePath)
{
	root->viewChild(this, FileSystem::Path(absoluteFilePath).begin(), Application::instance()->mainWindow().plugins());
	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), Qt::AscendingOrder);
}

void DirectoryView::setupModel(FileSystem::INode *root, const QString &absoluteFilePath, const Geometry &geometry)
{
	root->viewChild(this, FileSystem::Path(absoluteFilePath).begin(), Application::instance()->mainWindow().plugins());

	for (Geometry::size_type i = 0, size = qMin(m_node->columnCount(), geometry.size()); i < size; ++i)
		m_view.setColumnWidth(i, geometry.at(i));

	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), Qt::AscendingOrder);
}

void DirectoryView::select(const QModelIndex &index)
{
	m_view.scrollTo(index, QAbstractItemView::PositionAtCenter);
	m_view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	m_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
	m_view.setFocus();
}

void DirectoryView::setNode(FileSystem::INode *node, QAbstractItemModel *model, QAbstractItemDelegate *delegate)
{
    m_node = node;
	m_view.setModel(model);
	m_view.setItemDelegate(delegate);
	m_header.pathEdit.setText(m_node->absoluteFilePath());
	m_parent->updateTitle(this, m_node->fileName());
}

QString DirectoryView::rootPath()
{
#ifdef Q_OS_WIN
	return QString::fromLatin1("C:\\");
#else
	return QString::fromLatin1("/");
#endif
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
	for (qint32 i = 0, size = m_node->columnCount(); i < size; ++i)
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
	m_node->viewAbsolute(this, filePath, Application::instance()->mainWindow().plugins());
	m_node->refresh();
}

void DirectoryView::goUp()
{
	m_node->viewParent(this);
	m_parent->updateTitle(this, m_node->fileName());
}

void DirectoryView::goBack()
{

}

void DirectoryView::goForward()
{

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
		m_node->viewChild(this, index, Application::instance()->mainWindow().plugins());
		refresh();
	}
}

void DirectoryView::pathToClipboard()
{
	m_node->pathToClipboard(selectedIndexes());
}

void DirectoryView::rename()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		if (FileSystem::IFileInfo *info = m_node->info(index))
		{
			StringDialog dialog(
					info->isDir() ?
							tr("Enter new name for directory \"%1\"").arg(info->fileName()) :
							tr("Enter new name for file \"%1\"").arg(info->fileName()),
					tr("Name"),
					info->fileName(),
					this);

			m_parent->skipOneRefreshTab();

			if (dialog.exec() == QDialog::Accepted)
			{
				QString error;
				PScopedPointer<FileSystem::IFileControl> entry;
				PScopedPointer<FileSystem::IFileControl> control(m_node->createControl());

				if (entry = control->open(info->fileName(), error))
					if (entry->rename(dialog.value(), error))
						m_node->refresh();
					else
						QMessageBox::critical(this,
								info->isDir() ?
										tr("Failed to rename directory \"%1\"").arg(info->fileName()) :
										tr("Failed to rename file \"%1\"").arg(info->fileName()),
										error);
				else
					QMessageBox::critical(this,
							info->isDir() ?
									tr("Failed to rename directory \"%1\"").arg(info->fileName()) :
									tr("Failed to rename file \"%1\"").arg(info->fileName()),
									error);
			}
		}
}

void DirectoryView::createDirectory()
{
	StringDialog dialog(
			tr("Enter name for the new directory"),
			tr("Name"),
			QString(),
			this);

	m_parent->skipOneRefreshTab();

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;
		PScopedPointer<FileSystem::IFileControl> control(m_node->createControl());
		PScopedPointer<FileSystem::IFileControl> folder(control->create(dialog.value(), FileSystem::IFileControl::Folder, error));

		if (folder)
			m_node->refresh();
		else
			QMessageBox::critical(this, tr("Failed to create directory..."), error);
	}
}

void DirectoryView::remove()
{
	m_node->remove(selectedIndexes());
}

void DirectoryView::calculateSize()
{
	m_node->calculateSize(selectedIndexes());
}

void DirectoryView::copy()
{
	m_node->copy(selectedIndexes(), static_cast<DirectoryView*>(m_parent->other().currentWidget())->m_node);
}

void DirectoryView::move()
{
	m_node->move(selectedIndexes(), static_cast<DirectoryView*>(m_parent->other().currentWidget())->m_node);
}

void DirectoryView::openInNewTab()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		m_parent->openInNewTab(m_node->root(), m_node->info(index)->absoluteFilePath(), geometry());
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
	QStringList selectedFiles;
//	m_menu.popup(m_model.selectedFiles(selectedIndexes(), selectedFiles), selectedFiles);
}

void DirectoryView::refreshOther()
{
	static_cast<DirectoryView*>(m_parent->other().currentWidget())->refresh();
}

QList<qint32> DirectoryView::geometry() const
{
	QList<qint32> res;
	res.reserve(m_node->columnCount());

	for (qint32 i = 0, size = m_node->columnCount(); i < size; ++i)
		res.push_back(m_view.columnWidth(i));

	return res;
}

QModelIndex DirectoryView::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

QModelIndexList DirectoryView::selectedIndexes() const
{
	return m_view.selectionModel()->selectedIndexes();
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
