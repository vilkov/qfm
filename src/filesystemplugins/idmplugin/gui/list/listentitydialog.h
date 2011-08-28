#ifndef LISTENTITYDIALOG_H_
#define LISTENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
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

private Q_SLOTS:
	void removeEntity();

private:
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
	QPushButton m_addEntity;
	QPushButton m_removeEntity;
	QPushButton m_addProperty;
	QPushButton m_removeProperty;
	QTreeView m_view;
	QDialogButtonBox m_buttonBox;
	EntitiesTreeModel m_model;
};

#endif /* LISTENTITYDIALOG_H_ */
