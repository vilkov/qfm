#ifndef IDM_CONSTRAINTQUERYDIALOG_H_
#define IDM_CONSTRAINTQUERYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../../containeres/idm_container.h"
#include "../../../storage/constraints/value/idm_constraint.h"


using namespace ::VFS::Plugins::Idm;

class ConstraintQueryDialog : public QDialog
{
	Q_OBJECT

public:
	ConstraintQueryDialog(const IdmContainer &container, const Entity::Property &property, QWidget *parent = 0);

	Constraint *takeConstraint(BaseConstraint *parent);
    virtual void accept();

private Q_SLOTS:
	void chooseValue();
    void updateValue(const QString &text);
    void updateValue(const EntityValue::Holder &value);
    void updateValue(int index);

private:
    void setEditFont(bool italic);

private:
	IdmContainer m_container;
	const Entity::Property &m_property;
	EntityValue::Holder m_value;
	QLabel m_label;
	QComboBox m_operator;
	QLineEdit m_edit;
	QPushButton m_choose;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
};

#endif /* IDM_CONSTRAINTQUERYDIALOG_H_ */
