#include "chooseentitydialog.h"
#include <QtCore/QSet>
#include <QtGui/QMessageBox>


ChooseEntityDialog::ChooseEntityDialog(const QString &title, const IdmContainer &container, const List &list, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_label(tr("Entity"), this),
	m_comboBox(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(title);

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

IdmEntity *ChooseEntityDialog::chooseFile(const IdmContainer &container, QWidget *parent)
{
	QSet<IdmEntity*> entities;

	for (IdmContainer::size_type i = 0, size = container.size(); i < size; ++i)
		if (container.at(i)->type() == Database::Path)
			entities.unite(container.at(i)->parents().toSet());

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
			ChooseEntityDialog dialog(tr("Choose entity"), container, entities.toList(), parent);

			if (dialog.exec() == ChooseEntityDialog::Accepted)
				return dialog.value();
		}

	return NULL;
}

IdmEntity *ChooseEntityDialog::chooseProperty(const IdmContainer &container, IdmEntity *entity, QWidget *parent)
{
	QSet<IdmEntity*> entities;

	for (IdmContainer::size_type i = 0, size = container.size(); i < size; ++i)
		if (container.at(i) != entity)
			entities.insert(container.at(i));

	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
		entities.remove(entity->at(i).entity);

	if (entities.isEmpty())
		QMessageBox::information(parent,
								 tr("Entities not found"),
								 tr("There is no entities which is not a property of \"%1\".").
								 	 arg(entity->name()));
	else
	{
		ChooseEntityDialog dialog(tr("Choose property for \"%1\"").arg(entity->name()), container, entities.toList(), parent);

		if (dialog.exec() == ChooseEntityDialog::Accepted)
			return dialog.value();
	}

	return NULL;
}
