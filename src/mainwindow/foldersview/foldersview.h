#ifndef FOLDERSVIEW_H_
#define FOLDERSVIEW_H_

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTabWidget>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
#include "directoryview.h"
#include "../../filesystem/filesystemrootnode.h"


class FoldersView : public QWidget
{
    Q_OBJECT

public:
    typedef const FoldersView & FoldersViewRef;
    struct Tab
    {
    	Tab(const DirectoryView::Tab &tab) :
			isActive(false),
			tab(tab)
    	{}

    	bool isActive;
    	DirectoryView::Tab tab;
    };
    typedef QList<Tab> TabList;

public:
    FoldersView(FileSystem::RootNode *root, const TabList &tabs, FoldersViewRef other, QWidget *parent = 0);

    void refresh();
    void updateTitle(const QString &fileName);
    void updateTitle(qint32 index, const QString &fileName);
    void openInNewTab(const FileSystem::Info &fileInfo, const QList<qint32> &geometry);
    void closeCurrentTab();
	void setFocus();

	FoldersViewRef other() const { return m_other; }
	QWidget *currentWidget() const { return m_tabWidget.currentWidget(); }
	void saveTabs(QXmlStreamWriter &stream) const;
	static TabList loadTabs(QXmlStreamReader &stream);

protected:
	QString rootPath() const;

private Q_SLOTS:
	void refreshTab(int index);

private:
	void doRefresh(QWidget *tab);

private:
	FileSystem::RootNode *m_root;
	bool m_doNotRefreshTab;
    QVBoxLayout m_layout;
    QTabWidget m_tabWidget;
    FoldersViewRef m_other;
};

#endif /* FOLDERSVIEW_H_ */
