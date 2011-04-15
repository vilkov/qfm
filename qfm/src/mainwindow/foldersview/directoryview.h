#ifndef DIRECTORYVIEW_H
#define DIRECTORYVIEW_H

#include <QtGui/QWidget>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include "foldersview.h"
#include "widgets/pathedit.h"
#include "filesystemmodel/filesystemmodel.h"
#include "filesystemmodel/filesystemproxymodel.h"
#include "filesystemmodel/filesystemdecorationdelegate.h"
#include "../../tools/events/imp/mouseeventhandler.h"
#include "../../tools/events/imp/keyboardeventhandler.h"
#include "../../tools/events/imp/mouseeventsource.h"
#include "../../tools/events/imp/keyboardeventsource.h"


class DirectoryView : public QWidget
{
    Q_OBJECT

public:
    DirectoryView(const QString &directory, FoldersView *parent);
    DirectoryView(const QFileInfo &fileInfo, FoldersView *parent);

	const QFileInfo &currentDirectoryInfo() const;

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
    void updateCurrentDirectory(const QFileInfo &info);

private:
	void initialize();

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
				KeyboardEventSource<
					EventSourceBase<
						QTreeView
					>
				>
			> DirectoryListView;
	typedef MouseDoubleClickEventHandler<
				KeyboardEventHandler<
					EventHandlerBase<
						DirectoryView
					>
				>
			> DirectoryListViewEventHandler;

private:
	FoldersView *m_parent;
    QVBoxLayout m_layout;
    PathEventHandler m_pathEventHandler;
    Header m_header;
    DirectoryListView m_view;
    FileSystemModel m_model;
//    FileSystemProxyModel m_proxy;
    FileSystemDecorationDelegate m_delegate;
    DirectoryListViewEventHandler m_eventHandler;
};

#endif // DIRECTORYVIEW_H
