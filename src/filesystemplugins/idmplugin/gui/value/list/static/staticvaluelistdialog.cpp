#include "staticvaluelistdialog.h"
#include "../../../../../../tools/pointers/pscopedpointer.h"
#include <QtGui/QMessageBox>


StaticValueListDialog::StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_handler(this),
	m_entity(query.entity()),
	m_edit(&m_handler, this),
	m_view(this),
	m_accept(this),
	m_model(container, query, this),
	m_proxy(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Values of \"%1\"").arg(m_entity->name()));

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_edit, 1);
	m_horizontalLayout.addWidget(&m_accept);

    connect(&m_accept, SIGNAL(clicked()), this, SLOT(setFilter()));
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Enter, &StaticValueListDialog::setFilter);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Return, &StaticValueListDialog::setFilter);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Escape, &StaticValueListDialog::clearFilter);

    m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(&m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(selectValue(QModelIndex)));

    m_view.setHeaderHidden(true);

	if (m_model.isValid())
	{
		m_proxy.setDynamicSortFilter(true);
		m_proxy.setSourceModel(&m_model);
		m_view.setModel(&m_proxy);
	}
	else
		QMessageBox::critical(this, windowTitle(), m_model.lastError());
}

IdmEntityValue *StaticValueListDialog::takeValue()
{
	QString text = m_edit.text();
	QModelIndex index = currentIndex();

	if (index.isValid() &&
		m_model.at(index.row())->value().toString().compare(text, Qt::CaseSensitive) == 0)
		return m_model.take(index);
	else
		return IdmValueReader::createValue(m_entity, IdmEntityValue::InvalidId, text);
}

void StaticValueListDialog::accept()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		if (m_model.at(index.row())->value().toString().compare(m_edit.text(), Qt::CaseSensitive) == 0)
		{
			QDialog::accept();
			return;
		}

	if (m_edit.text().isEmpty())
		QMessageBox::warning(this, windowTitle(), "You must enter or choose the value.");
	else
		QDialog::accept();
}

QModelIndex StaticValueListDialog::currentIndex() const
{
	return m_proxy.mapToSource(m_view.selectionModel()->currentIndex());
}

void StaticValueListDialog::setFilter()
{
	m_proxy.setFilter(m_edit.text());
}

void StaticValueListDialog::clearFilter()
{
	m_edit.clear();
	m_proxy.setFilter(QString());
}

void StaticValueListDialog::selectValue(const QModelIndex &index)
{
	m_edit.setText(m_model.at(index.row())->value().toString());
	m_edit.setFocus();
}
