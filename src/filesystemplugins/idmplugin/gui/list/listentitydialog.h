#ifndef LISTENTITYDIALOG_H_
#define LISTENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/entitiestreemodel.h"


using namespace FileSystem::Plugins::Idm;

class ListEntityDialog : public QDialog
{
public:
	ListEntityDialog(QWidget *parent = 0);

    virtual void accept();

private:
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
	QPushButton m_addEntity;
	QPushButton m_removeEntity;
	QPushButton m_addProperty;
	QPushButton m_removeProperty;
	QTreeView m_view;
	QDialogButtonBox m_buttonBox;
};

#endif /* LISTENTITYDIALOG_H_ */
