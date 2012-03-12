#include "nestedwidget.h"


NestedWidget::NestedWidget(const QString &title) :
	BaseNestedWidget(),
	m_splitter(),
	m_groupbox(title),
	m_hlayout(&m_groupbox),
	m_ok(QString::fromLatin1("<OK"), this),
	m_cancel(QString::fromLatin1("<Cancel"), this)
{
	m_groupbox.setAlignment(Qt::AlignCenter);

	m_hlayout.setMargin(1);
	m_hlayout.setSpacing(1);
	m_hlayout.addLayout(&m_vlayout);

	m_vlayout.setMargin(1);
	m_vlayout.setSpacing(1);
	m_vlayout.addWidget(&m_ok);
	m_vlayout.addWidget(&m_cancel);
}
