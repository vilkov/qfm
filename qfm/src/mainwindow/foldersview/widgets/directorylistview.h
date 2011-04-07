#ifndef DIRECTORYLISTVIEW_H_
#define DIRECTORYLISTVIEW_H_

#include <QtGui/QTreeView>
#include "../../../tools/events/eventhandler.h"


class DirectoryListView : public QTreeView
{
	Q_OBJECT

public:
	DirectoryListView(EventHandler *eventHandler, QWidget *parent = 0);

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    EventHandler *m_eventHandler;
};

#endif /* DIRECTORYLISTVIEW_H_ */
