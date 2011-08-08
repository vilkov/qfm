#include "entitieslist.h"
#include "editor/artifactedit.h"
#include "dialog/createartifactdialog.h"
#include "dialog/addartifactpropertydialog.h"
#include "model/items/artifactslistitem.h"

#include <QModelIndexList>
#include <QMessageBox>


FILE_SYSTEM_NS_BEGIN

EntitiesListWidget::EntitiesListWidget(IdmStorage *storage, QWidget *parent = 0) :
	QWidget(parent),
	m_manager(location)
{
	ui.setupUi(this);
	ui.treeView->setModel(&m_model);
	m_model.insertArtifacts(m_manager.artifacts().values());
}

void EntitiesListWidget::actCreate()
{
	CreateArtifactDialog dialog(this);

	if (dialog.exec() == QDialog::Accepted)
		m_model.appendArtifact(m_manager.createArtifact(dialog.name(), dialog.type()));
}

void EntitiesListWidget::actDelete()
{
	QModelIndexList list = ui.treeView->selectionModel()->selectedIndexes();

	if (!list.isEmpty())
	{
		Artifact *artifact = static_cast<EntitiesListWidgetItem*>(list[0].internalPointer())->artifact();
		m_manager.deleteArtifact(artifact);
		m_model.removeArtifact(artifact);
		delete artifact;
	}
}

void EntitiesListWidget::addTo()
{
	QModelIndexList list = ui.treeView->selectionModel()->selectedIndexes();

	if (!list.isEmpty())
	{
		Artifact *artifact = static_cast<EntitiesListWidgetItem*>(list[0].internalPointer())->artifact();

		if (artifact->isVirtual())
		{
			QMap<qint32, Artifact*> artifacts = m_manager.artifacts();
			artifacts.remove(artifact->id());
			AddArtifactPropertyDialog dialog(artifacts.values(), this);

			if (dialog.exec() == QDialog::Accepted)
			{
				m_manager.addArtifactProperty(artifact, artifacts.values()[dialog.selectedArtifactIndex()]);
				m_model.appendArtifact(artifact, artifacts.values()[dialog.selectedArtifactIndex()]);
			}
		}
		else
			QMessageBox::critical(this, Errors::error(), Errors::artifactNotVirtual(artifact->name()));
	}
}

void EntitiesListWidget::delFrom()
{
	QModelIndexList list = ui.treeView->selectionModel()->selectedIndexes();

	if (!list.isEmpty())
	{
		QModelIndex index = m_model.parent(list[0]);

		if (index.isValid())
		{
			Artifact *artifact = static_cast<EntitiesListWidgetItem*>(index.internalPointer())->artifact();
			Artifact *property = static_cast<EntitiesListWidgetItem*>(list[0].internalPointer())->artifact();

			m_manager.delArtifactProperty(artifact, property);
			m_model.removeArtifact(artifact, property);
		}
	}
}

void EntitiesListWidget::actOpen(const QModelIndex &index)
{
	Artifact *artifact = static_cast<EntitiesListWidgetItem*>(index.internalPointer())->artifact();
	ArtifactsData data(m_manager);
	EditorData editorData(data, data.load(artifact));
	ArtifactEdit editor(editorData, this);
	editor.exec();
}

FILE_SYSTEM_NS_END
