#include "default_searchdialog.h"

#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

SearchDialog::SearchDialog(QWidget *parent) :
	QDialog(parent),
	m_layout(this),
	m_label(tr("File name pattern"), this),
	m_editor(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	m_hLayout.setSpacing(3);
	m_hLayout.setMargin(3);

	m_hLayout.addWidget(&m_label);
	m_hLayout.addWidget(&m_editor);

	m_layout.setSpacing(3);
	m_layout.setMargin(3);

	m_layout.addLayout(&m_hLayout);
	m_layout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void SearchDialog::accept()
{
	if (m_editor.text().isEmpty())
		QMessageBox::warning(this, windowTitle(), tr("Enter file name pattern"));
	else
		QDialog::accept();
}

void SearchDialog::reject()
{
	QDialog::reject();
}

DEFAULT_PLUGIN_NS_END
