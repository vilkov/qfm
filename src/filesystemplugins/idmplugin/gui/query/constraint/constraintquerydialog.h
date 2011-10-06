#ifndef CONSTRAINTQUERYDIALOG_H_
#define CONSTRAINTQUERYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../../containeres/idmcontainer.h"


using namespace FileSystem::Plugins::Idm;

class ConstraintQueryDialog : public QDialog
{
	Q_OBJECT

public:
	ConstraintQueryDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent = 0);

    virtual void accept();

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
};

#endif /* CONSTRAINTQUERYDIALOG_H_ */
