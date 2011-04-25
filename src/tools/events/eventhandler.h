#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_


class QMouseEvent;
class QKeyEvent;
class QFocusEvent;
class QContextMenuEvent;

class EventHandler
{
public:
	virtual ~EventHandler() {}

	virtual bool mouseReleaseEvent(QMouseEvent *event) = 0;
	virtual bool mouseDoubleClickEvent(QMouseEvent *event) = 0;
	virtual bool keyboardEvent(QKeyEvent *event) = 0;
    virtual bool focusOutEvent(QFocusEvent *event) = 0;
    virtual bool contextMenuEvent(QContextMenuEvent *event) = 0;
};

#endif /* EVENTHANDLER_H_ */
