#ifndef CHOOSEVALUEDIALOG_H_
#define CHOOSEVALUEDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/choosevaluelmodel.h"
#include "../../../../containeres/idmcontainer.h"
#include "../../../../storage/queries/idmselectquery.h"


using namespace FileSystem::Plugins::Idm;

class ChooseValueDialog : public QDialog
{
	Q_OBJECT

public:
	ChooseValueDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

    virtual void accept();

protected:
    QModelIndex currentIndex() const;

private:
	Select m_query;
	QLineEdit m_edit;
	QTreeView m_view;
	ChooseValueModel m_model;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* CHOOSEVALUEDIALOG_H_ */
