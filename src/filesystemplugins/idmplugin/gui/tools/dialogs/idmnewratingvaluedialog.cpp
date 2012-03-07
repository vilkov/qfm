#include "idmnewratingvaluedialog.h"


NewRatingValueDialog::NewRatingValueDialog(const QString &title, QWidget *parent) :
	QDialog(parent),
	m_result(1),
	m_button1(tr("1 - Bad"), this),
	m_button2(tr("2 - So-so"), this),
	m_button3(tr("3 - Good"), this),
	m_button4(tr("4 - Very good"), this),
	m_button5(tr("5 - Excellent"), this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(title);

	m_button1.setChecked(true);

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_button1);
	m_verticatLayout.addWidget(&m_button2);
	m_verticatLayout.addWidget(&m_button3);
	m_verticatLayout.addWidget(&m_button4);
	m_verticatLayout.addWidget(&m_button5);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_button1, SIGNAL(toggled(bool)), this, SLOT(button1Click(bool)));
    connect(&m_button2, SIGNAL(toggled(bool)), this, SLOT(button2Click(bool)));
    connect(&m_button3, SIGNAL(toggled(bool)), this, SLOT(button3Click(bool)));
    connect(&m_button4, SIGNAL(toggled(bool)), this, SLOT(button4Click(bool)));
    connect(&m_button5, SIGNAL(toggled(bool)), this, SLOT(button5Click(bool)));

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void NewRatingValueDialog::button1Click(bool checked)
{
	if (checked)
		m_result = 1;
}

void NewRatingValueDialog::button2Click(bool checked)
{
	if (checked)
		m_result = 2;
}

void NewRatingValueDialog::button3Click(bool checked)
{
	if (checked)
		m_result = 3;
}

void NewRatingValueDialog::button4Click(bool checked)
{
	if (checked)
		m_result = 4;
}

void NewRatingValueDialog::button5Click(bool checked)
{
	if (checked)
		m_result = 5;
}
