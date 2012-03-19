#ifndef NESTEDWIDGET_H_
#define NESTEDWIDGET_H_

#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include "basenestedwidget.h"


class NestedWidget : public BaseNestedWidget
{
public:
    enum DialogCode
    {
    	Rejected,
    	Accepted
    };

public:
	NestedWidget(const QString &title, NestedDialog *parent);
	virtual ~NestedWidget();

	/* BaseNestedWidget */
	virtual QWidget *centralWidget();
	virtual void setReadOnly(bool value);

	QString title() const { return m_groupbox.title(); }

protected:
	void addWidget(QWidget *widget, int stretch = 0) { m_hlayout.addWidget(widget, stretch); }
    void addLayout(QLayout *layout, int stretch = 0) { m_hlayout.addLayout(layout, stretch); }

private:
	QGroupBox m_groupbox;
	QHBoxLayout m_hlayout;
	QVBoxLayout m_vlayout;
	QPushButton m_ok;
	QPushButton m_cancel;
};

#endif /* NESTEDWIDGET_H_ */
