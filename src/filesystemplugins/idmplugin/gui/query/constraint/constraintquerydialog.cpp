#include "constraintquerydialog.h"
#include "../../value/list/static/staticvaluelistdialog.h"


ConstraintQueryDialog::ConstraintQueryDialog(const IdmContainer &container, const QString &name, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_entity(entity),
	m_label(name, this),
	m_operator(this),
	m_edit(this),
	m_choose(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Constraint on \"%1\" property").arg(name));

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_label);
	m_horizontalLayout.addWidget(&m_operator);
	m_horizontalLayout.addWidget(&m_edit, 1);
	m_horizontalLayout.addWidget(&m_choose);

    connect(&m_choose, SIGNAL(clicked()), this, SLOT(chooseValue()));

    m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    switch (m_entity->type())
    {
    	case Database::Int:
    	case Database::Date:
    	case Database::Time:
    	case Database::DateTime:
    	case Database::Rating:
    	{
    	    for (int op = Constraint::Less; op < Constraint::Like; ++op)
    	    	m_operator.addItem(Constraint::operatorToString(static_cast<Constraint::Operator>(op)), op);

    	    break;
    	}
    	case Database::String:
    	case Database::Memo:
    	case Database::Path:
    	{
	    	m_operator.addItem(Constraint::operatorToString(Constraint::Like), Constraint::Like);
	    	m_operator.addItem(Constraint::operatorToString(Constraint::Equal), Constraint::Equal);

	    	break;
    	}
    	case Database::Composite:
    		break;
    }

    m_operator.setCurrentIndex(0);
}

Constraint *ConstraintQueryDialog::takeConstraint()
{
	return 0;
}

void ConstraintQueryDialog::accept()
{
	QDialog::accept();
}

void ConstraintQueryDialog::chooseValue()
{
	StaticValueListDialog dialog(m_container, Select(m_entity), this);

	if (dialog.exec() == StaticValueListDialog::Accepted)
	{

	}
}
