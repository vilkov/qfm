#include "constraintquerydialog.h"
#include "../../value/list/static/staticvaluelistdialog.h"
#include "../../../../../tools/metatemplates.h"
#include <QtGui/QMessageBox>


ConstraintQueryDialog::ConstraintQueryDialog(const IdmContainer &container, const IdmEntity::Property &property, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_property(property),
	m_value(0),
	m_label(m_property.name, this),
	m_operator(this),
	m_edit(this),
	m_choose(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Constraint on \"%1\" property").arg(m_property.name));

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_label);
	m_horizontalLayout.addWidget(&m_operator);
	m_horizontalLayout.addWidget(&m_edit, 1);
	m_horizontalLayout.addWidget(&m_choose);

    connect(&m_choose, SIGNAL(clicked()), this, SLOT(chooseValue()));
    connect(&m_edit, SIGNAL(textEdited(QString)), this, SLOT(updateValue(QString)));
    connect(&m_operator, SIGNAL(currentIndexChanged(int)), this, SLOT(updateValue(int)));

    m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    switch (m_property.entity->type())
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
    	{
	    	m_operator.addItem(Constraint::operatorToString(Constraint::Equal), Constraint::Equal);

    		break;
    	}
    }

    m_operator.setCurrentIndex(0);
}

Constraint *ConstraintQueryDialog::takeConstraint(BaseConstraint *parent)
{
	if (m_value)
		return new Constraint(
				m_property,
				static_cast<Constraint::Operator>(m_operator.itemData(m_operator.currentIndex(), Qt::UserRole).toInt()),
				m_value,
				parent);
	else
		return new Constraint(
				m_property,
				static_cast<Constraint::Operator>(m_operator.itemData(m_operator.currentIndex(), Qt::UserRole).toInt()),
				IdmValueReader::createValue(m_property.entity, IdmEntityValue::InvalidId, m_edit.text()),
				parent);
}

void ConstraintQueryDialog::accept()
{
	if (m_edit.text().isEmpty())
		QMessageBox::warning(this, windowTitle(), "You must enter or choose the value.");
	else
		QDialog::accept();
}

void ConstraintQueryDialog::chooseValue()
{
	StaticValueListDialog dialog(m_container, Select(m_property.entity), this);

	if (dialog.exec() == StaticValueListDialog::Accepted)
		updateValue(dialog.takeValue());

	m_container.updateListGeometry(m_property.entity, dialog.geometry());
}

void ConstraintQueryDialog::updateValue(const QString &text)
{
	setEditFont(false);
	m_value.reset();
}

void ConstraintQueryDialog::updateValue(const IdmEntityValue::Holder &value)
{
	if (m_operator.itemData(m_operator.currentIndex(), Qt::UserRole).toInt() == Constraint::Equal)
		m_value.reset();
	else
		for (int i = 0, size = m_operator.count(); i < size; ++i)
			if (m_operator.itemData(i, Qt::UserRole).toInt() == Constraint::Equal)
			{
				m_operator.setCurrentIndex(i);
				break;
			}

	setEditFont(true);
	m_edit.setText((m_value = value)->value().toString());
}

void ConstraintQueryDialog::updateValue(int index)
{
	setEditFont(false);
	m_value.reset();
}

void ConstraintQueryDialog::setEditFont(bool italic)
{
	if (m_edit.font().italic() != italic)
	{
		QFont font(m_edit.font());

		font.setItalic(italic);
		m_edit.setFont(font);
	}
}
