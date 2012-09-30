#ifndef FOLDERSVIEW_H_
#define FOLDERSVIEW_H_

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTabWidget>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
#include "directoryview.h"
#include "../../tools/settings/options/settingsscope.h"
#include "../../tools/settings/options/settingsoptionlist.h"
#include "../../tools/settings/options/value/settingsoptionvalue.h"
#include "../../tools/settings/options/value/settingsoptionvaluelist.h"


class FoldersView : public QWidget
{
    Q_OBJECT

public:
    typedef const FoldersView & FoldersViewRef;

public:
    FoldersView(const QString &id, ::Tools::Settings::Scope *settings, FoldersViewRef other, QWidget *parent = 0);

    void refresh();
    void updateTitle(QWidget *widget, const QString &fileName);
    void openInNewTab(::VFS::INode *node, const QModelIndex &index, const QList<qint32> &geometry);
    void closeCurrentTab();
	void setFocus();
	void setCurrentDirectory(const QString &filePath);

	void skipOneRefreshTab();

	FoldersViewRef other() const { return m_other; }
	QWidget *currentWidget() const { return m_tabWidget.currentWidget(); }
	QPoint listPos() const { return static_cast<DirectoryView*>(m_tabWidget.currentWidget())->listPos(); }
    bool hasFocus() const { return static_cast<DirectoryView*>(m_tabWidget.currentWidget())->hasFocus(); }

private Q_SLOTS:
	void refreshTab(int index);

private:
	void doRefresh(QWidget *tab);

private:
	void save();
	void load();

private:
	class Sort : public ::Tools::Settings::Scope
	{
	public:
		Sort(Option *parent);
		Sort(const DirectoryView::Tab::Sort &sort, Option *parent);

		qint32 column() const { return m_column.value().toInt(); }
		Qt::SortOrder order() const { return static_cast<Qt::SortOrder>(m_order.value().toInt()); }

	private:
		::Tools::Settings::OptionValue m_column;
		::Tools::Settings::OptionValue m_order;
	};


	class Tab : public ::Tools::Settings::Scope
    {
    public:
    	Tab(Option *parent);
		Tab(const DirectoryView::Tab &tab, Option *parent);

		QString path() const { return m_path.value(); }
		const Sort &sort() const { return m_sort; }
		DirectoryView::Geometry geometry() const;
		const QString &currentFile() const { return m_currentFile.value(); }

    private:
		::Tools::Settings::OptionValue m_path;
		Sort m_sort;
		::Tools::Settings::OptionValueList m_geometry;
		::Tools::Settings::OptionValue m_currentFile;
    };


    class Tabs : public ::Tools::Settings::OptionList
    {
    public:
    	Tabs(Option *parent);

    	void add(const DirectoryView::Tab &tab);

    protected:
    	virtual bool isSubOptionName(const QStringRef &name) const;
    	virtual Option *create();
    };


    class Settings : public ::Tools::Settings::Scope
    {
    public:
    	typedef void (FoldersView::*Method)();

    public:
    	Settings(const QString &id, Scope *parent, FoldersView *object, Method save, Method load);

    	qint32 activeTab() const { return m_activeTab.value().toInt(); }
    	void setActiveTab(qint32 value) { m_activeTab.setValue(QString::number(value)); }

    	const Tabs &tabs() const { return m_tabs; }
    	Tabs &tabs() { return m_tabs; }

    protected:
    	virtual void save(QXmlStreamWriter &stream) const;
    	virtual void load(QXmlStreamReader &stream);
        virtual void loadDefault();

    private:
    	::Tools::Settings::OptionValue m_activeTab;
    	Tabs m_tabs;
    	FoldersView *m_object;
    	Method m_save;
    	Method m_load;
    };

private:
    Settings m_settings;
	bool m_doNotRefreshTab;
    QVBoxLayout m_layout;
    QTabWidget m_tabWidget;
    FoldersViewRef m_other;
};

#endif /* FOLDERSVIEW_H_ */
