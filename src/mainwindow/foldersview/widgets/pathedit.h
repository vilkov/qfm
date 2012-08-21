#ifndef PATHEDIT_H_
#define PATHEDIT_H_

#include <QtGui/QLineEdit>
#include "../../../tools/events/qt/eventhandler.h"


class PathEdit : public QLineEdit
{
	Q_OBJECT

public:
	PathEdit(::Tools::Events::EventHandler *eventHandler, QWidget *parent = 0);

	void startEdit();
	void cancelEdit();

protected:
	virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

private:
    ::Tools::Events::EventHandler *m_eventHandler;
    QString m_backupText;
};

#endif /* PATHEDIT_H_ */
