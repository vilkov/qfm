#ifndef EDITCOMPOSITEVALUEDIALOG_H_
#define EDITCOMPOSITEVALUEDIALOG_H_

#include <QtCore/QStringList>
#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../model/compositevaluemodel.h"
#include "../../../containeres/idmcontainer.h"
#include "../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class EditCompositeValueDialog : public QDialog
{
	Q_OBJECT

public:
	EditCompositeValueDialog(const IdmContainer &container, IdmCompositeEntityValue *value, QWidget *parent = 0);

    virtual void accept();
    IdmEntityValue *value();

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
					EditCompositeValueDialog
				>
			> TreeViewHandler;

	virtual void addValue();
	virtual void removeValue();

protected:
	void doAddValue(const QModelIndex &index);
	void doRemoveValue(const QModelIndex &index);

	const CompositeValueModel &model() const { return m_model; }
	CompositeValueModel &model() { return m_model; }

private:
	IdmContainer m_container;
	IdmCompositeEntityValue *m_value;
	TreeViewHandler m_handler;
	TreeView m_view;
	CompositeValueModel m_model;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* EDITCOMPOSITEVALUEDIALOG_H_ */
