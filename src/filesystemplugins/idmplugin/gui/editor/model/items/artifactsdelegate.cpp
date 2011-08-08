#include "artifactsdelegate.h"
#include "../../artifactedit.h"
#include "../../../dialog/texteditdialog.h"

#include <QSpinBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QDebug>


ArtifactsDelegate::ArtifactsDelegate(EditorData &data, QObject *parent) :
	QItemDelegate(parent),
	m_internal(data)
{}

QWidget *ArtifactsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch (m_internal.instance().artifact()->type())
	{
		case Artifact::Int:
			return new QSpinBox(parent);

		case Artifact::String:
			return new QLineEdit(parent);

		case Artifact::Date:
		{
			QDateTimeEdit *editor = new QDateTimeEdit(parent);
			editor->setDisplayFormat("dd.MM.yyyy");

			return editor;
		}

		case Artifact::Time:
		{
			QDateTimeEdit *editor = new QDateTimeEdit(parent);
			editor->setDisplayFormat("hh:mm:ss");

			return editor;
		}

		case Artifact::DateTime:
		{
			QDateTimeEdit *editor = new QDateTimeEdit(parent);
			editor->setDisplayFormat("dd.MM.yyyy hh:mm:ss");

			return editor;
		}

		case Artifact::Memo:
		{
			TextEditDialog dialog(index.model()->data(index, Qt::EditRole).toString(), parentWidget());

			if (dialog.exec() == QDialog::Accepted)
				updateInstance(index, dialog.text());

			return 0;
		}

		case Artifact::Virtual:
		{
			ArtifactsVirtualItem *item = static_cast<ArtifactsVirtualItem*>(index.internalPointer());
			EditorData data(m_internal.data(), item->column(index.column()).instance(), EditorData::Secondary);
			data.setParent(m_internal.instance().artifact(), item->id());

			ArtifactEdit editor(data, parentWidget());
			editor.exec();

			return 0;
		}

		default:
			return 0;
	}
}

template <>
qint32 ArtifactsDelegate::data(const QModelIndex &index) const
{
	return static_cast<ArtifactsSingleItem*>(index.internalPointer())->data().toInt();
}

template <>
QString ArtifactsDelegate::data(const QModelIndex &index) const
{
	return static_cast<ArtifactsSingleItem*>(index.internalPointer())->data().toString();
}

template <>
QDate ArtifactsDelegate::data(const QModelIndex &index) const
{
	return static_cast<ArtifactsSingleItem*>(index.internalPointer())->data().toDate();
}

template <>
QTime ArtifactsDelegate::data(const QModelIndex &index) const
{
	return static_cast<ArtifactsSingleItem*>(index.internalPointer())->data().toTime();
}

template <>
QDateTime ArtifactsDelegate::data(const QModelIndex &index) const
{
	return static_cast<ArtifactsSingleItem*>(index.internalPointer())->data().toDateTime();
}

template <typename T>
T ArtifactsDelegate::data(const QModelIndex &index) const
{
	return T();
}

void ArtifactsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	switch (m_internal.instance().artifact()->type())
	{
		case Artifact::Int:
			static_cast<QSpinBox*>(editor)->setValue(data<qint32>(index));
			break;

		case Artifact::String:
			static_cast<QLineEdit*>(editor)->setText(data<QString>(index));
			break;

		case Artifact::Date:
			static_cast<QDateTimeEdit*>(editor)->setDate(data<QDate>(index));
			break;

		case Artifact::Time:
			static_cast<QDateTimeEdit*>(editor)->setTime(data<QTime>(index));
			break;

		case Artifact::DateTime:
			static_cast<QDateTimeEdit*>(editor)->setDateTime(data<QDateTime>(index));
			break;

		case Artifact::Memo:
		case Artifact::Virtual:
			break;
	}
}

void ArtifactsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	switch (m_internal.instance().artifact()->type())
	{
		case Artifact::Int:
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		    spinBox->interpretText();
			updateInstance(index, spinBox->value());
		    break;
		}

		case Artifact::String:
			updateInstance(index, static_cast<QLineEdit*>(editor)->text());
			break;

		case Artifact::Date:
			updateInstance(index, static_cast<QDateTimeEdit*>(editor)->date());
			break;

		case Artifact::Time:
			updateInstance(index, static_cast<QDateTimeEdit*>(editor)->time());
			break;

		case Artifact::DateTime:
			updateInstance(index, static_cast<QDateTimeEdit*>(editor)->dateTime());
			break;

		case Artifact::Memo:
		case Artifact::Virtual:
			break;
	}
}

void ArtifactsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void ArtifactsDelegate::updateInstance(const QModelIndex &index, const QVariant &value) const
{
	ArtifactsSingleItem *item = static_cast<ArtifactsSingleItem*>(index.internalPointer());
	m_internal.manager().setArtifactInstance(m_internal.instance().artifact(), item->id(), value);
	item->setData(value);
}
