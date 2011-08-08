#include "addartifactpropertydialog.h"


AddArtifactPropertyDialog::AddArtifactPropertyDialog(const QList<Artifact*> &artifacts, QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);

	for (QList<Artifact*>::size_type i = 0; i < artifacts.size(); ++i)
		ui.artifactComboBox->addItem(artifacts[i]->name());
}

AddArtifactPropertyDialog::~AddArtifactPropertyDialog()
{

}
