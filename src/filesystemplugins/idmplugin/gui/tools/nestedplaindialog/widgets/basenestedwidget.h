#ifndef BASENESTEDWIDGET_H_
#define BASENESTEDWIDGET_H_

#include <QtCore/QObject>
#include <QtCore/QString>


class NestedDialog;
class NestedWidget;

class BaseNestedWidget : public QObject
{
	Q_OBJECT

public:
	BaseNestedWidget(NestedDialog *parent);
	virtual ~BaseNestedWidget();

	virtual QWidget *centralWidget() = 0;
	virtual void setReadOnly(bool value) = 0;
	virtual void setFocus() = 0;

protected:
	NestedDialog *parent() { return m_parent; }
	int open(NestedWidget *widget);
	void critical(const QString &text);
	void critical(const QString &title, const QString &text);

protected Q_SLOTS:
	friend class BaseNestedDialog;
	virtual void accept();
	virtual void reject();

private:
	NestedDialog *m_parent;
};

#endif /* BASENESTEDWIDGET_H_ */
