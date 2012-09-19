#include "idm_chooseentitydialog.h"

#include <vfs/model/vfs_proxymodel.h>

#include <QtCore/QSet>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>


template <>
class qLess<IdmEntity *>
{
public:
	typedef IdmEntity * value_type;

public:
    inline bool operator()(const value_type &t1, const value_type &t2) const
    {
        return ::VFS::ProxyModel::compareFileNames(t1->name(), t2->name());
    }
};


class ChoosePropertyDialog : public ChooseEntityDialog
{
public:
	ChoosePropertyDialog(const QString &title, const IdmContainer &container, const List &list, QWidget *parent = 0) :
		ChooseEntityDialog(title, container, list, parent),
		m_label(tr("Name"), this),
		m_edit(this)
	{
		m_horizontalLayout.setMargin(3);
		m_horizontalLayout.setSpacing(1);
		m_horizontalLayout.addWidget(&m_label);
		m_horizontalLayout.addWidget(&m_edit, 1);

		m_verticatLayout.insertLayout(1, &m_horizontalLayout);
	}

	QString name() const { return m_edit.text(); }

    virtual void accept()
    {
    	if (m_edit.text().isEmpty())
    		QMessageBox::warning(this, tr("There is empty fields"), tr("Field \"Name\" is empty!"));
    	else
    		ChooseEntityDialog::accept();
    }

private:
	QHBoxLayout m_horizontalLayout;
	QLabel m_label;
	QLineEdit m_edit;
};


ChooseEntityDialog::ChooseEntityDialog(const QString &title, const IdmContainer &container, const List &list, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_verticatLayout(this),
	m_label(tr("Entity"), this),
	m_comboBox(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(title);

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_label);
	m_horizontalLayout.addWidget(&m_comboBox, 1);

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
	QSet<IdmEntity *> entities;

	for (IdmContainer::size_type i = 0, size = container.size(); i < size; ++i)
		if (container.at(i)->type() == Database::Path)
			entities.unite(container.at(i)->parents().toSet());

	if (entities.isEmpty())
		QMessageBox::information(parent,
								 tr("Entities not found"),
								 tr("There is no entities with a property of type \"%1\".").
								 	 arg(container.entityTypes().value(Database::Path).label));
	else
		if (entities.size() == 1)
			return *entities.begin();
		else
		{
			List list(entities.toList());

			entities.clear();
			qSort(list);

			ChooseEntityDialog dialog(tr("Choose entity"), container, list, parent);

			if (dialog.exec() == ChooseEntityDialog::Accepted)
				return dialog.value();
		}

	return NULL;
}

IdmEntity *ChooseEntityDialog::chooseProperty(const IdmContainer &container, IdmEntity *entity, QString &propertyName, QWidget *parent)
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
		List list(entities.toList());

		entities.clear();
		qSort(list);

		ChoosePropertyDialog dialog(tr("Choose a property for \"%1\"").arg(entity->name()), container, list, parent);

		if (dialog.exec() == ChooseEntityDialog::Accepted)
		{
			propertyName = dialog.name();
			return dialog.value();
		}
	}

	return NULL;
}
