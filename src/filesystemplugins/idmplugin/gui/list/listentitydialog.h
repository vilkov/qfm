#ifndef LISTENTITYDIALOG_H_
#define LISTENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/entitiestreemodel.h"
#include "../../containeres/idmcontainer.h"


using namespace FileSystem::Plugins::Idm;

class ListEntityDialog : public QDialog
{
	Q_OBJECT

public:
	ListEntityDialog(const IdmContainer *container, QWidget *parent = 0);

    virtual void accept();

private:
	enum Actions
	{
		Create,
		Remove,
		AddProperty,
		RemoveProperty
	};

private Q_SLOTS:
	void actionTriggered(QAction *action);

private:
	QToolBar m_toolBar;
	QTreeView m_view;
	EntitiesTreeModel m_model;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* LISTENTITYDIALOG_H_ */
