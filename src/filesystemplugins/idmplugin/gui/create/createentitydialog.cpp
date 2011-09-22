#include "createentitydialog.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"
#include <QtGui/QMessageBox>


CreateEntityDialog::CreateEntityDialog(const IdmContainer &container, const QString &name, QWidget *parent) :
	QDialog(parent),
	m_label(this),
	m_lineEdit(name, this),
    m_label2(this),
    m_lineEdit2(this),
    m_label3(this),
    m_comboBox(this),
    m_view(this),
    m_addEntity(tr("Add"), this),
    m_removeEntity(tr("Remove"), this),
	m_gridLayout(this),
    m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_model(this),
	m_delegate(container)
{
	setWindowTitle(tr("Create a new entity"));
	setListEnabled(false);

    QFont font;
    font.setBold(true);
    font.setWeight(75);
    m_label.setFont(font);
    m_label.setText(tr("Entity name"));
    m_label2.setFont(font);
    m_label2.setText(tr("Short format"));
    m_label3.setFont(font);
    m_label3.setText(tr("Select type"));

    m_gridLayout.setMargin(3);
    m_gridLayout.setSpacing(1);
    m_gridLayout.addWidget(&m_label,       0, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit,    0, 1, 1, 1);
    m_gridLayout.addWidget(&m_label2,      1, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit2,   1, 1, 1, 1);
    m_gridLayout.addWidget(&m_label3,      2, 0, 1, 1);
    m_gridLayout.addWidget(&m_comboBox,    2, 1, 1, 1);
    m_gridLayout.addLayout(&m_gridLayout2, 3, 0, 1, 2);
    m_gridLayout.addWidget(&m_buttonBox,   4, 0, 1, 2);

    m_horizontalLayout.setMargin(3);
    m_horizontalLayout.setSpacing(1);
    m_horizontalLayout.addWidget(&m_addEntity);
    m_horizontalLayout.addWidget(&m_removeEntity);

    m_gridLayout2.setMargin(3);
    m_gridLayout2.setSpacing(1);
    m_gridLayout2.addLayout(&m_horizontalLayout, 0, 0, 1, 1);
    m_gridLayout2.addWidget(&m_view, 1, 0, 1, 1);

    connect(&m_addEntity, SIGNAL(clicked()), this, SLOT(add()));
    connect(&m_removeEntity, SIGNAL(clicked()), this, SLOT(remove()));

    m_view.setHeaderHidden(true);
    m_view.setModel(&m_model);
    m_view.setItemDelegate(&m_delegate);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_lineEdit.selectAll();

    for (IdmEntityTypes::const_iterator it = container.entityTypes().constBegin(), end = container.entityTypes().constEnd(); it != end; ++it)
    	m_comboBox.addItem(it->label, it.key());

    m_comboBox.setCurrentIndex(0);
    connect(&m_comboBox, SIGNAL(activated(int)), this, SLOT(activated(int)));
}

void CreateEntityDialog::accept()
{
	if (name().isEmpty())
		QMessageBox::warning(this, windowTitle(), tr("You must enter the name!"));
	else
	{
		if (m_lineEdit2.isEnabled())
		{
			IdmShortFormat format = shortFormat();

			if (format.isValid())
			{
				bool ok;

				for (IdmShortFormat::size_type i = 0, size = format.size(); i < size; ++i)
					if (format.at(i).type() == IdmShortFormat::Token::Property)
					{
						ok = false;

						for (size_type q = 0, size = m_model.size(); q < size; ++q)
							if (m_model.nameAt(q).compare(format.at(i).string(), Qt::CaseSensitive) == 0)
							{
								ok = true;
								break;
							}

						if (!ok)
						{
							QMessageBox::warning(this, windowTitle(), tr("Short format points to property \"%1\" which is not in properties of this entity!").arg(format.at(i).string()));
							return;
						}
					}
			}
			else
			{
				QMessageBox::warning(this, windowTitle(), tr("Short format is invalid (%1)!").arg(format.lastError()));
				return;
			}
		}

		for (size_type i = 0, q, size = m_model.size(); i < size; ++i)
			for (q = i + 1; q < size; ++q)
				if (m_model.entityAt(q)->id() == m_model.entityAt(i)->id())
				{
					QMessageBox::warning(this, windowTitle(), tr("More than one property of type \"%1\"!").arg(m_model.entityAt(i)->name()));
					return;
				}
				else
					if (m_model.nameAt(q).compare(m_model.nameAt(i), Qt::CaseInsensitive) == 0)
					{
						QMessageBox::warning(this, windowTitle(), tr("There is a properties with the same names \"%1\"!").arg(m_model.nameAt(i)));
						return;
					}

		QDialog::accept();
	}
}

void CreateEntityDialog::activated(int index)
{
	setListEnabled(m_comboBox.itemData(index, Qt::UserRole).toInt() == Database::Composite);
}

void CreateEntityDialog::add()
{
	if (m_delegate.container().size() == 0)
		QMessageBox::warning(this, windowTitle(), tr("There is no entities!"));
	else
	{
		StringDialog dialog(tr("New property name"), tr("Name"), QString(), this);

		if (dialog.exec() == StringDialog::Accepted)
			m_model.add(m_delegate.container().at(0), dialog.value());
	}
}

void CreateEntityDialog::remove()
{
	QModelIndex index = m_view.currentIndex();

	if (index.isValid())
		m_model.remove(index);
}

void CreateEntityDialog::setListEnabled(bool enabled)
{
    m_view.setEnabled(enabled);
    m_addEntity.setEnabled(enabled);
    m_removeEntity.setEnabled(enabled);
    m_label2.setEnabled(enabled);
    m_lineEdit2.setEnabled(enabled);
}
