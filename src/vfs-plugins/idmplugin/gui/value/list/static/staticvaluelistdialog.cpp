#include "staticvaluelistdialog.h"


StaticValueListDialog::StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	NestedPlainDialog(parent),
	m_handler(this),
	m_widget(&m_handler, container, query, this)
{
	const QRect &geometry = query.entity()->listGeometry();

	if (geometry.isValid())
		setGeometry(geometry);

	setWindowTitle(tr("Values of \"%1\"").arg(query.entity()->name()));

    m_handler.registerShortcut(Qt::CTRL, Qt::Key_F, &StaticValueListDialog::setFocusToFilter);
    m_widget.setViewToolTip(tr("CTRL+F - activate filter field"));

    setCentralWidget(&m_widget);
}

IdmEntityValue::Holder StaticValueListDialog::takeValue()
{
	return m_widget.takeValue();
}

void StaticValueListDialog::accept()
{
	if (currentIndex().isValid())
		QDialog::accept();
	else
		warning(windowTitle(), "You must choose the value.");
}

QModelIndex StaticValueListDialog::currentIndex() const
{
	return m_widget.currentIndex();
}

void StaticValueListDialog::setFocusToFilter()
{
	m_widget.setFocusToFilter();
}
