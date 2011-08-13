#ifndef FOLDERSVIEW_H_
#define FOLDERSVIEW_H_

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTabWidget>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
#include "directoryview.h"
#include "../../filesystem/info/filesystemfoldernodeinfo.h"


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
    FoldersView(FileSystem::INode *root, const TabList &tabs, FoldersViewRef other, QWidget *parent = 0);

    void refresh();
    void updateTitle(QWidget *widget, const QString &fileName);
    void openInNewTab(FileSystem::INode *root, const QString &absoluteFilePath, const QList<qint32> &geometry);
    void closeCurrentTab();
	void setFocus();
	void setCurrentDirectory(const QString &filePath);

	void skipOneRefreshTab();

	FoldersViewRef other() const { return m_other; }
	QWidget *currentWidget() const { return m_tabWidget.currentWidget(); }
	QPoint listPos() const { return static_cast<DirectoryView*>(m_tabWidget.currentWidget())->listPos(); }
    bool hasFocus() const { return static_cast<DirectoryView*>(m_tabWidget.currentWidget())->hasFocus(); }

	void saveTabs(QXmlStreamWriter &stream) const;
	static TabList loadTabs(QXmlStreamReader &stream);

private Q_SLOTS:
	void refreshTab(int index);

private:
	void doRefresh(QWidget *tab);

private:
	bool m_doNotRefreshTab;
    QVBoxLayout m_layout;
    QTabWidget m_tabWidget;
    FoldersViewRef m_other;
};

#endif /* FOLDERSVIEW_H_ */
