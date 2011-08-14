#include "createentitydialog.h"
#include <QtGui/QMessageBox>


CreateEntityDialog::CreateEntityDialog(const FileSystem::IdmItemsList *entities, const FileSystem::IdmEntityTypes &entityTypes, const QString &name, QWidget *parent) :
	QDialog(parent),
	m_label(this),
	m_lineEdit(name, this),
    m_label2(this),
    m_comboBox(this),
    m_view(this),
    m_addEntity(tr("Add"), this),
    m_removeEntity(tr("Remove"), this),
	m_gridLayout(this),
	m_buttonBox(this),
	m_model(entities)
{
	setWindowTitle(tr("Create a new entity"));
	setListEnabled(false);

    QFont font;
    font.setBold(true);
    font.setWeight(75);
    m_label.setFont(font);
    m_label.setText(tr("Entity name"));
    m_label2.setFont(font);
    m_label2.setText(tr("Select type"));

    m_buttonBox.setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    m_gridLayout.setSpacing(6);
    m_gridLayout.setContentsMargins(6, 6, 6, 6);
    m_gridLayout2.setSpacing(6);
    m_gridLayout2.setContentsMargins(6, 6, 6, 6);
    m_horizontalLayout.setSpacing(6);
    m_horizontalLayout.setContentsMargins(6, 6, 6, 6);

    m_gridLayout.addWidget(&m_label,       0, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit,    0, 1, 1, 1);
    m_gridLayout.addWidget(&m_label2,      1, 0, 1, 1);
    m_gridLayout.addWidget(&m_comboBox,    1, 1, 1, 1);
    m_gridLayout.addLayout(&m_gridLayout2, 2, 0, 1, 2);
    m_gridLayout.addWidget(&m_buttonBox,   3, 0, 1, 2);

    m_horizontalLayout.addWidget(&m_addEntity);
    m_horizontalLayout.addWidget(&m_removeEntity);
    m_gridLayout2.addLayout(&m_horizontalLayout, 0, 0, 1, 1);
    m_gridLayout2.addWidget(&m_view, 1, 0, 1, 1);

    m_view.setModel(&m_model);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_lineEdit.selectAll();

    for (FileSystem::IdmEntityTypes::const_iterator it = entityTypes.constBegin(), end = entityTypes.constEnd(); it != end; ++it)
    	m_comboBox.addItem(it->label, it.key());

    m_comboBox.setCurrentIndex(0);
    connect(&m_comboBox, SIGNAL(activated(int)), this, SLOT(activated(int)));
}

void CreateEntityDialog::accept()
{
	if (name().isEmpty())
		QMessageBox::information(this, windowTitle(), tr("You must enter the name!"));
	else
		QDialog::accept();
}

void CreateEntityDialog::activated(int index)
{
	setListEnabled(m_comboBox.itemData(index, Qt::UserRole).toInt() == FileSystem::IdmEntity::Composite);
}

void CreateEntityDialog::setListEnabled(bool enabled)
{
    m_view.setEnabled(enabled);
    m_addEntity.setEnabled(enabled);
    m_removeEntity.setEnabled(enabled);
}
