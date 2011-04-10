#ifndef FOLDERSVIEW_H_
#define FOLDERSVIEW_H_

#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include "widgets/pathedit.h"
#include "../../tools/events/imp/mouseeventhandler.h"
#include "../../tools/events/imp/focuseventhandler.h"
#include "../../tools/events/imp/keyboardeventhandler.h"


class FoldersView : public QWidget
{
    Q_OBJECT

public:
    typedef const FoldersView & FoldersViewRef;

public:
    FoldersView(const QStringList &folders, FoldersViewRef other, QWidget *parent = 0);

    void updateCurrentDirectory(const QFileInfo &info);
    void openInNewTab(const QFileInfo &fileInfo);
    void closeCurrentTab();
	void setFocus();

	FoldersViewRef other() const { return m_other; }
	QWidget *currentWidget() const { return m_tabWidget.currentWidget(); }

private:
    QVBoxLayout m_layout;
    QTabWidget m_tabWidget;
    FoldersViewRef m_other;
};

#endif /* FOLDERSVIEW_H_ */
