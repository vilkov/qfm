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
//#include "filesystemmodel/filesystemmodel.h"
//#include "filesystemmodel/filesystemproxymodel.h"
//#include "filesystemmodel/filesystemdecorationdelegate.h"
#include "../../filesystem/filesystemrootnode.h"
#include "../../filesystem/info/filesysteminfo.h"
#include "../../tools/events/imp/mouseeventhandler.h"
#include "../../tools/events/imp/keyboardeventhandler.h"
#include "../../tools/events/imp/contextmenueventhandler.h"
#include "../../tools/events/imp/mouseeventsource.h"
#include "../../tools/events/imp/keyboardeventsource.h"
#include "../../tools/events/imp/contextmenueventsource.h"
#include "../../tools/events/imp/focuseventhandler.h"


class FoldersView;

class DirectoryView : public QWidget
{
    Q_OBJECT

public:
    struct Tab
    {
    	struct Sort
    	{
    		qint32 column;
    		Qt::SortOrder order;
    	};

    	QString path;
    	Sort sort;
    	QList<qint32> geometry;
    };

public:
    DirectoryView(FileSystem::RootNode *root, const Tab &tab, FoldersView *parent);
    DirectoryView(FileSystem::RootNode *root, const FileSystem::Info &fileInfo, FoldersView *parent);
    DirectoryView(FileSystem::RootNode *root, const FileSystem::Info &fileInfo, const QList<qint32> &geometry, FoldersView *parent);

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

private:
	void openInNewTab();
	void closeTab();
    void editPath();
	void selectIndex(const QModelIndex &index);
    void updateCurrentDirectory(const QString &fileName, const QString &absoluteFilePath);
    void contextMenu();

private:
    friend class FileSystemModelBase;
    void refreshOther();
    QList<qint32> geometry() const;

private:
	void initialize(FileSystem::RootNode *root, const QString &filePath);
	FileSystem::Node *model() const { return static_cast<FileSystem::Node*>(static_cast<QAbstractProxyModel*>(m_view.model())->sourceModel()); }
	QModelIndex currentIndex() const;
	QModelIndexList selectedIndexes() const;
	QModelIndex toViewIndex(const QModelIndex &index) const;

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
	ContextMenu m_menu;
    QVBoxLayout m_layout;
    PathEventHandler m_pathEventHandler;
    Header m_header;
    DirectoryListView m_view;
//    FileSystemModel m_model;
//    FileSystemProxyModel m_proxy;
//    FileSystemDecorationDelegate m_delegate;
    DirectoryListViewEventHandler m_eventHandler;
};

#endif // DIRECTORYVIEW_H
