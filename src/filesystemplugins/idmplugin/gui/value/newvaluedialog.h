#ifndef NEWVALUEDIALOG_H_
#define NEWVALUEDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../containeres/idmcontainer.h"


using namespace FileSystem::Plugins::Idm;

class NewValueDialog : public QDialog
{
	Q_OBJECT

public:
	NewValueDialog(IdmContainer *container, IdmEntity *entity, QWidget *parent = 0);

    virtual void accept();

private:
	IdmContainer *m_container;
	IdmEntity *m_entity;
	QTreeView m_view;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* NEWVALUEDIALOG_H_ */
