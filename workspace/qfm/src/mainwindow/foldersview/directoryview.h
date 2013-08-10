/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DIRECTORYVIEW_H
#define DIRECTORYVIEW_H

#include <QtCore/QList>
#include <QtGui/QWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>

#include <vfs/view/vfs_nodeview.h>

#include <tools/events/qt/imp/mouseeventsource.h>
#include <tools/events/qt/imp/keyboardeventsource.h>
#include <tools/events/qt/imp/contextmenueventsource.h>

#include "widgets/pathedit.h"


using namespace ::Tools::Events;
class FoldersView;

class DirectoryView : public ::VFS::NodeView
{
    Q_OBJECT

public:
    typedef ::VFS::NodeView           BaseClass;
    typedef ::VFS::NodeView::Geometry Geometry;

    struct Tab
    {
    	struct Sort
    	{
    		qint32 column;
    		Qt::SortOrder order;
    	};

    	QString path;
    	Sort sort;
    	Geometry geometry;
    	QString currentFile;
    };

public:
    DirectoryView(const QString &absoluteFilePath, FoldersView *parent);
    DirectoryView(::VFS::INode *node, const QModelIndex &index, const Geometry &geometry, FoldersView *parent);
    DirectoryView(const QString &path, qint32 column, Qt::SortOrder order, const Geometry &geometry, const QString &currentFile, FoldersView *parent);
    virtual ~DirectoryView();

public:
	/* INodeView */
	virtual ::VFS::INode *node() const;
	virtual ::VFS::INodeView *opposite() const;
	virtual QModelIndex currentIndex() const;
	virtual QModelIndexList selectedIndexes() const;
	virtual void edit(const QModelIndex &index);
	virtual void select(const QModelIndex &index);
	virtual void setNode(::VFS::INode *node);

	static QString defaultPath();

    bool hasFocus() const;
	QPoint listPos() const;
	QString title() const;

	Tab tab() const;

	void setFocus();
	void setCurrentDirectory(const QString &filePath);

public Q_SLOTS:
	/* ::VFS::NodeView */
	void goUp();
    void goBackward();
    void goForward();
    void refresh();
	void cancel();

protected:
	/* ::VFS::NodeView */
	virtual bool handleShortcut(QEvent *event);

private Q_SLOTS:
	void actionTriggered(QAction *action);

private:
	void openInNewTab();
	void closeTab();
    void editPath();

private:
    friend class FileSystemModelBase;
    QList<qint32> geometry() const;

private:
	typedef MouseReleaseEventHandler<
				FocusOutEventHandler<
					KeyboardEventHandler<
						EventHandlerBase<
							DirectoryView
						>
					>
				>
			> PathEventHandler;

private:
	struct Header
	{
		Header(PathEventHandler *eventHandler, QWidget *parent);

		QHBoxLayout layout;
		PathEdit pathEdit;
		bool pathEditFlag;
		QToolButton favorites;
		QToolButton history;
	};
private Q_SLOTS:
	void actHeaderPathAccept();
	void actHeaderPathReject();
	void actHeaderFavorites();
	void actHeaderHistory();

private:
	void initialize();

private:
	typedef MouseDoubleClickEventSource<
				ContextMenuEventSource<
					KeyboardEventSource<
						EventSourceBase<
							QTreeView
						>
					>
				>
			> DirectoryListView;

	typedef KeyboardEventHandler<
				EventHandlerBase<
					DirectoryView
				>
			> DirectoryListViewEventHandler;

private:
	FoldersView *m_parent;
    ::VFS::INode *m_node;
    QVBoxLayout m_layout;
    DirectoryListViewEventHandler m_eventHandler;
    PathEventHandler m_pathEventHandler;
    Header m_header;
    QToolBar m_toolBar;
    DirectoryListView m_view;
};

#endif // DIRECTORYVIEW_H
