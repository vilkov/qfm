#include "settingsdialog.h"
#include "../configoptions/configoptionvalue.h"


Dialog::Dialog(const QString &title, DialogSettings &settings, const Pages &pages, QWidget *parent) :
	QDialog(parent),
	m_settings(settings),
	m_layout(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_model(pages, this)
{
	setWindowTitle(title);
	restoreGeometry(settings.geometry());
	setFont(settings.font());

	m_pagesView.setModel(&m_model);
	m_pagesView.setHeaderHidden(true);
	m_selectedPage.setLayout(m_editedPages[pages.at(0)] = pages.at(0)->createEditor());

	m_hLayout.setSpacing(1);
    m_hLayout.setMargin(1);

    m_hLayout.addWidget(&m_pagesView);
	m_hLayout.addWidget(&m_selectedPage);

	m_layout.setSpacing(3);
	m_layout.setMargin(1);

	m_layout.addLayout(&m_hLayout);
	m_layout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

Dialog::~Dialog()
{
	qDeleteAll(m_editedPages);
}

void Dialog::accept()
{
	for (PagesCache::const_iterator i = m_editedPages.constBegin(), end = m_editedPages.constEnd(); i != end; ++i)
		if (!i.key()->accept())
			return;

	QDialog::accept();
}

void Dialog::reject()
{
	for (PagesCache::const_iterator i = m_editedPages.constBegin(), end = m_editedPages.constEnd(); i != end; ++i)
		i.key()->reject();

	QDialog::reject();
}



SettingsDialog::SettingsDialog(const QString &title, const SettingsList &settings, const ConstraintsList &constraints, QWidget *parent) :
	QDialog(parent),
    m_tabs(this),
	m_layout(this),
	m_buttonBox(this),
	m_constraints(constraints)
{
    setWindowTitle(title);

    m_buttonBox.setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_layout.setSpacing(6);
    m_layout.setContentsMargins(6, 6, 6, 6);
    m_layout.addWidget(&m_tabs);
    m_layout.addWidget(&m_buttonBox);

    QFont font = createFont();
	for (SettingsList::size_type i = 0, size = settings.size(); i < size; ++i)
		m_tabs.addTab(settings.at(i)->createGroup(font), settings.at(i)->title());
}

void SettingsDialog::accept()
{
	for (ConstraintsList::size_type i = 0, size = m_constraints.size(); i < size; ++i)
		if (!m_constraints.at(i)->check(this))
			return;

	QDialog::accept();
}

QFont SettingsDialog::createFont() const
{
    QFont font;
    font.setBold(true);
    font.setWeight(75);

    return font;
}
