#include "directoryview.h"
#include "foldersview.h"
#include "../../tools/widgets/stringdialog/stringdialog.h"
#include <QtGui/QHeaderView>
#include <QtGui/QMessageBox>


DirectoryView::DirectoryView(FileSystem::RootNode *root, const Tab &tab, FoldersView *parent) :
	QWidget(),
	m_parent(parent),
	m_menu(this),
    m_layout(this),
    m_pathEventHandler(this),
    m_header(&m_pathEventHandler, this),
    m_view(&m_eventHandler, this),
	m_eventHandler(this)
{
	initialize();
	setupModel(root, tab);
}

DirectoryView::DirectoryView(FileSystem::RootNode *root, const FileSystem::Info &fileInfo, FoldersView *parent) :
	QWidget(),
	m_parent(parent),
	m_menu(this),
	m_layout(this),
    m_pathEventHandler(this),
    m_header(&m_pathEventHandler, this),
	m_view(&m_eventHandler, this),
	m_eventHandler(this)
{
	initialize();
	setupModel(root, fileInfo.absoluteFilePath());
}

DirectoryView::DirectoryView(FileSystem::RootNode *root, const FileSystem::Info &fileInfo, const QList<qint32> &geometry, FoldersView *parent) :
	QWidget(),
	m_parent(parent),
	m_menu(this),
	m_layout(this),
	m_pathEventHandler(this),
	m_header(&m_pathEventHandler, this),
	m_view(&m_eventHandler, this),
	m_eventHandler(this)
{
	initialize();
	setupModel(root, fileInfo.absoluteFilePath(), geometry);
}

QString DirectoryView::currentDirectoryName() const
{
	return model()->fileName();
}

void DirectoryView::save(QXmlStreamWriter &stream) const
{
	stream.writeTextElement(QString::fromLatin1("Path"), model()->absoluteFilePath());

	stream.writeStartElement(QString::fromLatin1("Sort"));
	stream.writeTextElement(QString::fromLatin1("Column"), QString::number(m_view.header()->sortIndicatorSection()));
	stream.writeTextElement(QString::fromLatin1("Order"), QString::number(m_view.header()->sortIndicatorOrder()));
	stream.writeEndElement();

	QString name = QString::fromLatin1("Column");
	stream.writeStartElement(QString::fromLatin1("Geometry"));
	for (qint32 i = 0, size = model()->columnCount(); i < size; ++i)
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
	QModelIndex index;

	m_parent->root()->view(filePath, &m_view, index);

	if (index.isValid())
		selectIndex(index);
	else
		if ((index = model()->rootIndex()).isValid())
			selectIndex(index);

	updateCurrentDirectory(model()->fileName(), model()->absoluteFilePath());
}

void DirectoryView::goUp()
{
	FileSystem::Node *model = DirectoryView::model();
	QModelIndex index = model->rootIndex();

	if (index.isValid())
	{
		FileSystem::Node *node = model->subnode(index, m_parent->root()->plugins());

		node->view(&m_view);
		node->update();
		updateCurrentDirectory(node->fileName(), node->absoluteFilePath());

		if ((index = model->parentEntryIndex()).isValid())
			selectIndex(index);
		else
			if ((index = node->rootIndex()).isValid())
				selectIndex(index);
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
	FileSystem::Node *model = DirectoryView::model();
	model->refresh();

	if (model->exists())
		model->update();
	else
	{
		FileSystem::Node *parent = model;

		do
		{
			parent = static_cast<FileSystem::Node*>(model->parent());
			parent->remove(model);
			parent->refresh();
			model = parent;
		}
		while (!model->exists());

		model->view(&m_view);
		updateCurrentDirectory(model->fileName(), model->absoluteFilePath());
	}
}

void DirectoryView::activated()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
	{
		FileSystem::Node *model = DirectoryView::model();

		if (FileSystem::Node *node = model->subnode(index, m_parent->root()->plugins()))
		{
			if (model->isRootIndex(index))
				index = model->parentEntryIndex();
			else
				index = node->rootIndex();

			node->view(&m_view);
			node->update();
			updateCurrentDirectory(node->fileName(), node->absoluteFilePath());
			selectIndex(index);
		}
	}
}

void DirectoryView::pathToClipboard()
{
//	m_model.pathToClipboard(selectedIndexes());
}

void DirectoryView::rename()
{
//	QModelIndex index = currentIndex();
//
//	if (index.isValid())
//	{
//		const FileSystemInfo &info = m_model.fileInfo(index);
//
//		if (info.exists())
//		{
//			QString fileName = info.fileName();
//
//			StringDialog dialog(
//					info.isDir() ?
//							tr("Enter new name for directory \"%1\"").arg(fileName) :
//							tr("Enter new name for file \"%1\"").arg(fileName),
//					tr("Name"),
//					fileName,
//					this);
//
//			if (dialog.exec() == QDialog::Accepted)
//				m_model.rename(index, dialog.value());
//		}
//	}
}

void DirectoryView::createDirectory()
{
//	StringDialog dialog(
//			tr("Enter name for directory"),
//			tr("Name"),
//			QString(),
//			this);
//
//	if (dialog.exec() == QDialog::Accepted)
//		m_model.createDirectory(dialog.value());
}

void DirectoryView::remove()
{
//	QModelIndex index = currentIndex();
//
//	if (index.isValid())
//		m_model.remove(index);
}

void DirectoryView::calculateSize()
{
	model()->calculateSize(selectedIndexes());
}

void DirectoryView::copy()
{
	model()->copy(selectedIndexes(), static_cast<DirectoryView*>(m_parent->other().currentWidget())->model());
}

void DirectoryView::move()
{
//	QModelIndex index = currentIndex();
//
//	if (index.isValid())
//		m_model.move(index, &static_cast<DirectoryView*>(m_parent->other().currentWidget())->m_model);
}

void DirectoryView::openInNewTab()
{
//	QModelIndex index = currentIndex();
//
//	if (index.isValid())
//	{
//		const FileSystemInfo &info = m_model.fileInfo(index);
//
//		if (info.isDir())
//			m_parent->openInNewTab(info, geometry());
//	}
}

void DirectoryView::closeTab()
{
//	QStringList list = m_model.lockedEntries();
//
//	if (list.isEmpty())
//		m_parent->closeCurrentTab();
//	else
//		QMessageBox::information(
//				this,
//				tr("Unfinished tasks..."),
//				tr("There is a busy entries:\n").append(list.join(QChar('\n'))));
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

void DirectoryView::updateCurrentDirectory(const QString &fileName, const QString &absoluteFilePath)
{
	m_header.pathEdit.setText(absoluteFilePath);
	m_parent->updateTitle(fileName);
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
	res.reserve(model()->columnCount());

	for (qint32 i = 0, size = model()->columnCount(); i < size; ++i)
		res.push_back(m_view.columnWidth(i));

	return res;
}

void DirectoryView::initialize()
{
	setLayout(&m_layout);
	m_layout.setMargin(1);
	m_layout.addLayout(&m_header.layout);
	m_layout.addWidget(&m_view);

	m_view.setSelectionMode(QAbstractItemView::SingleSelection);
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

void DirectoryView::setupModel(FileSystem::RootNode *root, const Tab &tab)
{
	QModelIndex index;

	root->view(tab.path, &m_view, index);
	m_header.pathEdit.setText(model()->absoluteFilePath());

	if (index.isValid())
		selectIndex(index);

	for (qint32 i = 0, size = qMin(model()->columnCount(), tab.geometry.size()); i < size; ++i)
		m_view.setColumnWidth(i, tab.geometry.at(i));

	m_view.sortByColumn(tab.sort.column, tab.sort.order);
}

void DirectoryView::setupModel(FileSystem::RootNode *root, const QString &absoluteFilePath)
{
	QModelIndex index;

	root->view(absoluteFilePath, &m_view, index);
	m_header.pathEdit.setText(model()->absoluteFilePath());

	if (index.isValid())
		selectIndex(index);

	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), Qt::AscendingOrder);
}

void DirectoryView::setupModel(FileSystem::RootNode *root, const QString &absoluteFilePath, const QList<qint32> &geometry)
{
	QModelIndex index;

	root->view(absoluteFilePath, &m_view, index);
	m_header.pathEdit.setText(model()->absoluteFilePath());

	if (index.isValid())
		selectIndex(index);

	for (qint32 i = 0, size = qMin(model()->columnCount(), geometry.size()); i < size; ++i)
		m_view.setColumnWidth(i, geometry.at(i));

	m_view.sortByColumn(m_view.header()->sortIndicatorSection(), Qt::AscendingOrder);
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
