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
	NestedWidget(NestedDialog *parent, const QString &title);
	virtual ~NestedWidget();

	/* BaseNestedWidget */
	virtual QWidget *centralWidget();
	virtual void setReadOnly(bool value);

	int exec();

	QString title() const { return m_groupbox.title(); }

protected:
	void addWidget(QWidget *widget, int stretch = 0) { m_hlayout.addWidget(widget, stretch); }
    void addLayout(QLayout *layout, int stretch = 0) { m_hlayout.addLayout(layout, stretch); }

private:
    class Button : public QPushButton
    {
    public:
    	Button(const QString &text);

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;
    };

private:
	QGroupBox m_groupbox;
	QHBoxLayout m_hlayout;
	QVBoxLayout m_vlayout;
	Button m_ok;
	Button m_cancel;
};

#endif /* NESTEDWIDGET_H_ */