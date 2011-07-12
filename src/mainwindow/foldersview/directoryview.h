#ifndef DIRECTORYVIEW_H
#define DIRECTORYVIEW_H

#include <QtCore/QList>
#include <QtGui/QWidget>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
#include "widgets/pathedit.h"
#include "contextmenu/contextmenu.h"
#include "../../filesystem/interfaces/filesysteminode.h"
#include "../../tools/events/imp/mouseeventhandler.h"
#include "../../tools/events/imp/keyboardeventhandler.h"
#include "../../tools/events/imp/contextmenueventhandler.h"
#include "../../tools/events/imp/mouseeventsource.h"
#include "../../tools/events/imp/keyboardeventsource.h"
#include "../../tools/events/imp/contextmenueventsource.h"
#include "../../tools/events/imp/focuseventhandler.h"


class FoldersView;

class DirectoryView : public QWidget, public FileSystem::INodeView
{
    Q_OBJECT

public:
    typedef QList<qint32> Geometry;

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
    };

public:
    DirectoryView(FoldersView *parent);
    virtual ~DirectoryView();

    /* Should be called immediately after construction and adding to the tabWidget! */
	void setupModel(FileSystem::INode *root, const Tab &tab);
	void setupModel(FileSystem::INode *root, const QString &absoluteFilePath);
	void setupModel(FileSystem::INode *root, const QString &absoluteFilePath, const Geometry &geometry);

public:
	/* INodeView */
	virtual void select(const QModelIndex &index);
	virtual void setNode(FileSystem::INode *node, QAbstractItemModel *model, QAbstractItemDelegate *delegate = 0);

	static QString rootPath();

	QPoint listPos() const;
	QString currentDirectoryName() const;
	void save(QXmlStreamWriter &stream) const;
	static Tab load(QXmlStreamReader &stream, const QString &stopTagName);

	void setFocus();
	void setCurrentDirectory(const QString &filePath);

public Q_SLOTS:
	void goUp();
    void goBack();
    void goForward();
    void refresh();
    void activated();
	void pathToClipboard();
	void rename();
	void createDirectory();
	void remove();
	void calculateSize();
	void copy();
	void move();
	void cancel();

private:
	void openInNewTab();
	void closeTab();
    void editPath();
    void contextMenu();

private:
    friend class FileSystemModelBase;
    void refreshOther();
    QList<qint32> geometry() const;

private:
	QModelIndex currentIndex() const;
	QModelIndexList selectedIndexes() const;

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
	typedef MouseDoubleClickEventSource<
				ContextMenuEventSource<
					KeyboardEventSource<
						EventSourceBase<
							QTreeView
						>
					>
				>
			> DirectoryListView;
	typedef MouseDoubleClickEventHandler<
				ContextMenuEventHandler<
					KeyboardEventHandler<
						EventHandlerBase<
							DirectoryView
						>
					>
				>
			> DirectoryListViewEventHandler;

private:
	FoldersView *m_parent;
    FileSystem::INode *m_node;
	ContextMenu m_menu;
    QVBoxLayout m_layout;
    PathEventHandler m_pathEventHandler;
    Header m_header;
    DirectoryListView m_view;
    DirectoryListViewEventHandler m_eventHandler;
};

#endif // DIRECTORYVIEW_H
