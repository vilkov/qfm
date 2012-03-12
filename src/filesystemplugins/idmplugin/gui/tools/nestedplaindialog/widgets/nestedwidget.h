#ifndef NESTEDWIDGET_H_
#define NESTEDWIDGET_H_

#include <QtGui/QSplitter>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include "basenestedwidget.h"


class NestedWidget : public BaseNestedWidget
{
public:
	NestedWidget(const QString &title);

	QSplitter *splitter() { return &m_splitter; }
	QWidget *centralWidget() { return &m_groupbox; }

protected:
	void addWidget(QWidget *widget) { m_hlayout.addWidget(widget); }

private:
	QSplitter m_splitter;
	QGroupBox m_groupbox;
	QHBoxLayout m_hlayout;
	QVBoxLayout m_vlayout;
	QPushButton m_ok;
	QPushButton m_cancel;
};

#endif /* NESTEDWIDGET_H_ */
