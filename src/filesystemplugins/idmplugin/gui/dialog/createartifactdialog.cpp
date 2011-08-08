#include "createartifactdialog.h"


CreateArtifactDialog::CreateArtifactDialog(QWidget *parent) :
	QDialog(parent),
	m_types(Artifact::types())
{
	ui.setupUi(this);
	ui.typeComboBox->addItems(m_types.keys());
}

void CreateArtifactDialog::accept()
{
	if (!ui.nameLineEdit->text().isEmpty())
		QDialog::accept();
}
