#include "createentitydialog.h"
#include <QtGui/QMessageBox>


CreateEntityDialog::CreateEntityDialog(const FileSystem::IdmItemsList *entities, const FileSystem::IdmEntityTypes &entityTypes, const QString &name, QWidget *parent) :
	QDialog(parent),
	m_label(this),
	m_lineEdit(name, this),
    m_label2(this),
    m_comboBox(this),
	m_gridLayout(this),
	m_buttonBox(this)
{
	setWindowTitle(tr("Create a new entity"));

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
    m_gridLayout.addWidget(&m_label,     0, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit,  0, 1, 1, 1);
    m_gridLayout.addWidget(&m_label2,    1, 0, 1, 1);
    m_gridLayout.addWidget(&m_comboBox,  1, 1, 1, 1);
    m_gridLayout.addWidget(&m_buttonBox, 2, 0, 1, 2);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_lineEdit.selectAll();

    for (FileSystem::IdmEntityTypes::const_iterator it = entityTypes.constBegin(), end = entityTypes.constEnd(); it != end; ++it)
    	m_comboBox.addItem(it->label, it.key());

    m_comboBox.setCurrentIndex(0);
}

void CreateEntityDialog::accept()
{
	if (name().isEmpty())
		QMessageBox::information(this, windowTitle(), tr("You must enter the name!"));
	else
		QDialog::accept();
}
