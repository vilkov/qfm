#ifndef IDM_COMPOSITEVALUEDIALOG_H_
#define IDM_COMPOSITEVALUEDIALOG_H_

#include "widgets/idm_compositevaluewidget.h"
#include "../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


class CompositeValueDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	CompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, QWidget *parent = 0);
	CompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent = 0);

protected:
	virtual void addValue();
	virtual void removeValue();

protected:
    QModelIndex currentIndex() const { return m_mainWidget.currentIndex(); }

    const IdmContainer &container() const { return m_mainWidget.container(); }
	IdmContainer &container() { return m_mainWidget.container(); }

	const CompositeValueModel &model() const { return m_mainWidget.model(); }
	CompositeValueModel &model() { return m_mainWidget.model(); }

	void doOpenFile(const QModelIndex &index) { m_mainWidget.open(index); }
	void doAddValue(const QModelIndex &index) { m_mainWidget.addValue(index); }
	void doRemoveValue(const QModelIndex &index) { m_mainWidget.removeValue(index); }

private:
	void openFile1();
	void openFile2();

private:
	typedef MouseDoubleClickEventHandler<
				KeyboardEventHandler<
					EventHandlerBase<
						CompositeValueDialog
					>
				>
			> TreeViewHandler;

private:
	TreeViewHandler m_handler;
	MainCompositeValueWidget m_mainWidget;
};

#endif /* IDM_COMPOSITEVALUEDIALOG_H_ */
