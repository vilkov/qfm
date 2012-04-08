#include "staticvaluelistdialog.h"
#include "../../../../../../tools/pointers/pscopedpointer.h"
#include "../../../../../../application.h"
#include <QtGui/QMessageBox>


StaticValueListDialog::StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_handler(this),
	m_entity(query.entity()),
	m_edit(&m_handler, this),
	m_view(this),
	m_accept(Application::desktopService()->searchIcon(16), QString(), this),
	m_model(container, query, this),
	m_proxy(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	const QRect &geometry = query.entity()->listGeometry();

	if (geometry.isValid())
		setGeometry(geometry);

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
		m_view.setSortingEnabled(true);
		m_view.sortByColumn(0, Qt::AscendingOrder);
		m_view.setModel(&m_proxy);
	}
	else
		QMessageBox::critical(this, windowTitle(), m_model.lastError());
}

IdmEntityValue::Holder StaticValueListDialog::takeValue()
{
	return m_model.take(currentIndex());
}

void StaticValueListDialog::accept()
{
	if (currentIndex().isValid())
		QDialog::accept();
	else
		QMessageBox::warning(this, windowTitle(), "You must choose the value.");
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
	m_edit.setText(m_model.at(m_proxy.mapToSource(index).row())->value().toString());
	m_edit.setFocus();
}
