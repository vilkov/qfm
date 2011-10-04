#include "choosevaluedialog.h"
#include "../new/composite/newcompositevaluedialog.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../tools/widgets/valuedialog/valuedialog.h"
#include <QtGui/QMessageBox>


ChooseValueDialog::ChooseValueDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_query(query),
	m_edit(this),
	m_view(this),
	m_model(container, m_query, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Values of \"%1\"").arg(query.entity()->name()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_view.setHeaderHidden(true);

	if (m_model.isValid())
		m_view.setModel(&m_model);
	else
		QMessageBox::critical(this, windowTitle(), m_model.lastError());
}

void ChooseValueDialog::accept()
{
	if (currentIndex().isValid())
		QDialog::accept();
	else
		QMessageBox::warning(this, windowTitle(), "You must select a value.");
}

QModelIndex ChooseValueDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}
