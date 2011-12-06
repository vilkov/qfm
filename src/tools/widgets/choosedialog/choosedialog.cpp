#include "choosedialog.h"
#include <QtGui/QMessageBox>


ChooseDialog::ChooseDialog(const QString &title, const List &values, QWidget *parent) :
	QDialog(parent),
	m_layout(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(title);
	m_values.reserve(values.size());

    QFont font;
    font.setBold(true);
    font.setWeight(75);

    m_layout.setSpacing(6);
    m_layout.setMargin(6);

    QRadioButton *button;
    for (List::size_type i = 0, size = values.size(); i < size; ++i)
    {
    	button = new QRadioButton(values.at(i).first, this);
    	button->setFont(font);

    	m_values.push_back(Container::value_type(values.at(i).second, button));
        m_layout.addWidget(button);
    }

    m_layout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void *ChooseDialog::value() const
{
    for (Container::size_type i = 0, size = m_values.size(); i < size; ++i)
    	if (m_values.at(i).second->isChecked())
    		return m_values.at(i).first;

    return 0;
}

void ChooseDialog::accept()
{
	if (check())
		QDialog::accept();
	else
		QMessageBox::information(this, windowTitle(), tr("You must choose one of the values!"));
}

bool ChooseDialog::check()
{
    for (Container::size_type i = 0, size = m_values.size(); i < size; ++i)
    	if (m_values.at(i).second->isChecked())
    		return true;

	return false;
}
