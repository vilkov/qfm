#include "idm_ratingvaluedialog.h"


RatingValueDialog::RatingValueDialog(const QString &title, QWidget *parent) :
	NestedPlainDialog(parent),
	m_mainWidget(this)
{
	setCentralWidget(&m_mainWidget);
}
