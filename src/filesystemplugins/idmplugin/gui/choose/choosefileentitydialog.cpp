#include "choosefileentitydialog.h"
#include <QtCore/QSet>
#include <QtGui/QMessageBox>


ChooseFileEntityDialog::ChooseFileEntityDialog(const IdmContainer &container, const List &list, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_label(tr("Entity"), this),
	m_comboBox(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Choose entity"));

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_label);
	m_horizontalLayout.addWidget(&m_comboBox);

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_buttonBox);

	connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	for (List::size_type i = 0, size = list.size(); i < size; ++i)
		m_comboBox.addItem(list.at(i)->name(), m_container.indexOf(list.at(i)->id()));

	m_comboBox.setCurrentIndex(0);
}

IdmEntity *ChooseFileEntityDialog::choose(const IdmContainer &container, QWidget *parent)
{
	QSet<IdmEntity*> entities;

	for (IdmContainer::size_type i = 0, size = container.size(); i < size; ++i)
		if (container.at(i)->type() == Database::Path)
		{
			const IdmEntity::Parents &parents = container.at(i)->parents();

			for (IdmEntity::Parents::size_type i = 0, size = parents.size(); i < size; ++i)
				entities.insert(parents.at(i));
		}

	if (entities.isEmpty())
		QMessageBox::information(parent,
								 tr("Entities not found"),
								 tr("There is no entities with property of type \"%1\".").
								 arg(container.entityTypes().value(Database::Path).label));
	else
		if (entities.size() == 1)
			return *entities.begin();
		else
		{
			ChooseFileEntityDialog dialog(container, entities.toList(), parent);

			if (dialog.exec() == ChooseFileEntityDialog::Accepted)
				return dialog.value();
		}

	return 0;
}
