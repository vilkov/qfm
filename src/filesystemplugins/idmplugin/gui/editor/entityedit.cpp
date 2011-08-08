#include "artifactedit.h"
#include "model/items/artifactscolumn.h"
#include "model/items/artifactssingleitem.h"
#include "model/items/artifactsvirtualitem.h"


ArtifactEdit::ArtifactEdit(EditorData &data, QWidget *parent, const bool buttonBox) :
	QDialog(parent),
	m_internal(data),
	m_delegate(data, this),
	m_model(header())
{
    ui.setupUi(this);

    if (!buttonBox)
    	ui.buttonBox->hide();

    if (m_internal.type() == EditorData::Master)
    {
    	static const QString windowTitle = tr("Artifact \"%1\"");
    	static const QString addInstanceToolButtonText = tr("Add instance");
    	static const QString delInstanceToolButtonText = tr("Del instance");

    	ui.addInstanceToolButton->setText(addInstanceToolButtonText);
    	ui.delInstanceToolButton->setText(delInstanceToolButtonText);
    	setWindowTitle(windowTitle.arg(m_internal.instance().artifact()->name()));
    }
    else
    {
    	static const QString windowTitle = tr("Artifact \"%1\" instances of artifact \"%2\"");
    	static const QString addInstanceToolButtonText = tr("Add instance to \"%1\"");
    	static const QString delInstanceToolButtonText = tr("Del instance from \"%1\"");

    	ui.addInstanceToolButton->setText(addInstanceToolButtonText.arg(m_internal.instance().artifact()->name()));
    	ui.delInstanceToolButton->setText(delInstanceToolButtonText.arg(m_internal.instance().artifact()->name()));
    	setWindowTitle(windowTitle.arg(m_internal.instance().artifact()->name()).arg(m_internal.parent().artifact->name()));
    }

	ui.treeView->setModel(&m_model);
	ui.treeView->setItemDelegate(&m_delegate);
	insertItems(m_internal.instance());
}

void ArtifactEdit::insertItems(ArtifactInstance &instance)
{
	insertItems(instance, instance.ids());
}

void ArtifactEdit::insertItems(ArtifactInstance &instance, const QList<qint32> &ids)
{
	QList<ModelItem*> items;

	if (instance.artifact()->isVirtual())
    	for (QList<qint32>::size_type i = 0; i < ids.size(); ++i)
		{
			QList<ArtifactsColumn> columns;
			QList<ArtifactInstance> &instances = instance.data<QList<ArtifactInstance> >(ids[i]);

			for (QList<qint32>::size_type q = 0; q < instances.size(); ++q)
			{
				if (!instances[q].isReference())
					instances[q] = m_internal.data().load(instances[q]);

				columns.push_back(ArtifactsColumn(instances[q]));
			}

			items.push_back(new ArtifactsVirtualItem(ids[i], columns));
		}
	else
		for (QList<qint32>::size_type i = 0; i < ids.size(); ++i)
			items.push_back(new ArtifactsSingleItem(ids[i], instance.data<QVariant>(ids[i])));

	if (!items.isEmpty())
		m_model.insertRows(0, items);
}

void ArtifactEdit::actAddInstance()
{
	if (m_internal.type() == EditorData::Master)
	{
		ArtifactInstance instance = m_internal.data().addInstance(m_internal.instance().artifact());
		m_internal.instance() += instance;
		insertItems(m_internal.instance(), instance.ids());
	}
	else
	{
		ArtifactInstance instance = m_internal.data().load(m_internal.instance().artifact());
		instance -= m_internal.instance();

		EditorData data(m_internal.data(), instance);
		ArtifactEdit editor(data, this, true);

		if (editor.exec() == QDialog::Accepted)
		{
			QList<qint32> dataIds;
			QModelIndexList list = editor.selection();

			for (QModelIndexList::size_type i = 0, row = -1; i < list.size() && list[i].row() != row; ++i)
			{
				ArtifactsItem *item = static_cast<ArtifactsItem*>(list[i].internalPointer());
				dataIds.push_back(item->id());
				row = item->row();
			}

			if (!dataIds.isEmpty())
			{
				m_internal.manager().addArtifactInstance(
						m_internal.parent().artifact,
						instance.artifact(),
						m_internal.parent().id,
						dataIds);

				m_internal.instance() += dataIds;
				insertItems(m_internal.instance(), dataIds);
			}
		}
	}
}

void ArtifactEdit::actDelInstance()
{
//	QModelIndexList list = selection();
//
//	if (!list.isEmpty())
//		if (m_internal.type() == EditorData::Master)
//		{
//			QList<qint32> dataIds;
//
//			for (QModelIndexList::size_type i = 0, row = -1; i < list.size() && list[i].row() != row; ++i)
//			{
//				ArtifactsItem *item = static_cast<ArtifactsItem*>(list[i].internalPointer());
//				dataIds.push_back(item->id());
//				row = item->row();
//			}
//
////			ArtifactInstance instance = m_internal.data().load(m_internal.instance().artifact(), dataIds);
//			m_internal.data().manager().delArtifactInstance(instance);
//			m_internal.instance() -= instance;
//			m_internal.data().del(instance);
//
//			for (QModelIndexList::size_type i = 0, row = -1; i < list.size() && list[i].row() != row; ++i)
//			{
//				ArtifactsItem *item = static_cast<ArtifactsItem*>(list[i].internalPointer());
//				row = item->row();
//				m_model.remove(item);
//			}
//		}
//		else
//		{
//			QList<qint32> dataIds;
//
//			for (QModelIndexList::size_type i = 0, row = -1; i < list.size() && list[i].row() != row; ++i)
//			{
//				ArtifactsItem *item = static_cast<ArtifactsItem*>(list[i].internalPointer());
//				dataIds.push_back(item->id());
//				row = item->row();
//			}
//
//			ArtifactInstance instance = m_internal.data().load(m_internal.instance().artifact(), dataIds);
//			m_internal.data().manager().delArtifactInstance(
//					m_internal.parent().artifact,
//					instance.artifact(),
//					m_internal.parent().id,
//					instance.ids());
//			m_internal.instance() -= instance;
//
//			for (QModelIndexList::size_type i = 0, row = -1; i < list.size() && list[i].row() != row; ++i)
//			{
//				ArtifactsItem *item = static_cast<ArtifactsItem*>(list[i].internalPointer());
//				row = item->row();
//				m_model.remove(item);
//			}
//		}
}

QList<QVariant> ArtifactEdit::header() const
{
    QList<QVariant> res;

    if (m_internal.instance().artifact()->isVirtual())
	{
    	QList<Artifact*> artifacts = m_internal.instance().propertys();

    	for (QList<Artifact*>::size_type i = 0; i < artifacts.size(); ++i)
    		res << artifacts[i]->name();
	}
	else
		res << m_internal.instance().artifact()->name();

    return res;
}
