#ifndef COMPOSITEVALUEDIALOG_H_
#define COMPOSITEVALUEDIALOG_H_

#include <QtCore/QStringList>
#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../model/compositevaluemodel.h"
#include "../../tools/nestedplaindialog/nestedplaindialog.h"
#include "../../../containeres/idmcontainer.h"
#include "../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class CompositeValueDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	CompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent = 0);

    virtual void accept();

protected:
    QModelIndex currentIndex() const;

protected:
	typedef KeyboardEventSource<
				EventSourceBase<
					QTreeView
				>
			> TreeView;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					CompositeValueDialog
				>
			> TreeViewHandler;

	virtual void addValue();
	virtual void removeValue();

protected:
	void doAddValue(const QModelIndex &index);
	void doRemoveValue(const QModelIndex &index);

	const IdmContainer &container() const { return m_container; }
	IdmContainer &container() { return m_container; }

	const CompositeValueModel &model() const { return m_model; }
	CompositeValueModel &model() { return m_model; }

private:
	IdmContainer m_container;
	IdmEntityValue::Holder m_value;
	TreeViewHandler m_handler;
	TreeView m_view;
	CompositeValueModel m_model;
//	QDialogButtonBox m_buttonBox;
//	QVBoxLayout m_verticatLayout;
};

#endif /* COMPOSITEVALUEDIALOG_H_ */
