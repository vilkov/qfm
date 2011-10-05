#ifndef STATICVALUELISTDIALOG_H_
#define STATICVALUELISTDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/staticvaluelistmodel.h"
#include "../model/valuelistproxymodel.h"
#include "../../../../containeres/idmcontainer.h"
#include "../../../../storage/queries/idmselectquery.h"
#include "../../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class StaticValueListDialog : public QDialog
{
	Q_OBJECT

public:
	StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

    virtual void accept();

protected:
    QModelIndex currentIndex() const;

private:
	typedef KeyboardEventSource<
				EventSourceBase<
					QLineEdit
				>
			> LineEdit;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					StaticValueListDialog
				>
			> LineEditHandler;

private Q_SLOTS:
	void setFilter();
	void clearFilter();

private:
	LineEditHandler m_handler;
	Select m_query;
	LineEdit m_edit;
	QTreeView m_view;
	QPushButton m_accept;
	StaticValueListModel m_model;
	ValueListProxyModel m_proxy;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
};

#endif /* STATICVALUELISTDIALOG_H_ */
