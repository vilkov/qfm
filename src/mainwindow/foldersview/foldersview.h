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
    FoldersView(const QStringList &folders, QWidget *parent = 0);

    void updateCurrentDirectory(const QFileInfo &info);
    void openInNewTab(const QFileInfo &fileInfo);
    void closeCurrentTab();
	void setFocus();

private:
    QVBoxLayout m_layout;
    QTabWidget m_tabWidget;
};

#endif /* FOLDERSVIEW_H_ */
