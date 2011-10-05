#include "staticvaluelistdialog.h"
#include <QtGui/QMessageBox>


StaticValueListDialog::StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_handler(this),
	m_query(query),
	m_edit(&m_handler, this),
	m_view(this),
	m_accept(this),
	m_model(container, m_query, this),
	m_proxy(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Values of \"%1\"").arg(query.entity()->name()));

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_edit);
	m_horizontalLayout.addWidget(&m_accept);

    connect(&m_accept, SIGNAL(setFilter()), this, SLOT(setFilter()));
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

void StaticValueListDialog::accept()
{
	if (currentIndex().isValid())
		QDialog::accept();
	else
		QMessageBox::warning(this, windowTitle(), "You must select a value.");
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
