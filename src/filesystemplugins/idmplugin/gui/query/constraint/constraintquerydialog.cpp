#include "constraintquerydialog.h"


ConstraintQueryDialog::ConstraintQueryDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_entity(entity)
{

}

void ConstraintQueryDialog::accept()
{

}
