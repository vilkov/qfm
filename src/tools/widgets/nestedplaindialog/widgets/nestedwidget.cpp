#include "nestedwidget.h"
#include "nesteddialog.h"


NestedWidget::NestedWidget(NestedDialog *parent, const QString &title) :
	BaseNestedWidget(parent),
	m_groupbox(title),
	m_hlayout(&m_groupbox),
	m_ok(QString::fromLatin1("<Y")),
	m_cancel(QString::fromLatin1("<N"))
{
	m_groupbox.setAlignment(Qt::AlignCenter);

	m_hlayout.setMargin(1);
	m_hlayout.setSpacing(1);
	m_hlayout.addLayout(&m_vlayout);

	m_vlayout.setMargin(1);
	m_vlayout.setSpacing(1);
	m_vlayout.addWidget(&m_ok);
	m_vlayout.addWidget(&m_cancel);

	connect(&m_ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(&m_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

NestedWidget::~NestedWidget()
{}

QWidget *NestedWidget::centralWidget()
{
	return &m_groupbox;
}

void NestedWidget::setReadOnly(bool value)
{
	m_groupbox.setEnabled(!value);
}

int NestedWidget::exec()
{
	return parent()->open(this);
}

NestedWidget::Button::Button(const QString &text) :
	QPushButton(text)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

QSize NestedWidget::Button::sizeHint() const
{
	QSize res(QPushButton::sizeHint());
	res.setWidth(20);
	return res;
}

QSize NestedWidget::Button::minimumSizeHint() const
{
	QSize res(QPushButton::minimumSizeHint());
	res.setWidth(20);
	return res;
}
