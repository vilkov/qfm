#include "newpropertyname.h"


NewPropertyNameDialog::NewPropertyNameDialog(const Properties &properties, QWidget *parent) :
	StringDialog(tr("New property name"), tr("Name"), QString(), parent),
	m_properties(properties)
{}

bool NewPropertyNameDialog::check()
{
	return false;
}
