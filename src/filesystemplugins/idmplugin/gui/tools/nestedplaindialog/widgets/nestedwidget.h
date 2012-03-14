#ifndef NESTEDWIDGET_H_
#define NESTEDWIDGET_H_

#include <QtCore/QObject>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>


class NestedDialog;

class NestedWidget : public QObject
{
	Q_OBJECT

public:
	NestedWidget(const QString &title, NestedDialog *parent);
	virtual ~NestedWidget();

	QWidget *centralWidget() { return &m_groupbox; }

	void setReadOnly(bool value) { m_groupbox.setEnabled(!value); }
	virtual void setFocus() = 0;

protected:
	void addWidget(QWidget *widget, int stretch = 0) { m_hlayout.addWidget(widget, stretch); }

protected Q_SLOTS:
	friend class BaseNestedDialog;
	virtual void accept();
	virtual void reject();

private:
	QGroupBox m_groupbox;
	QHBoxLayout m_hlayout;
	QVBoxLayout m_vlayout;
	QPushButton m_ok;
	QPushButton m_cancel;
	NestedDialog *m_parent;
};

#endif /* NESTEDWIDGET_H_ */
