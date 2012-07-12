#include "vfs_userinputdialog.h"

#include <QtGui/QStyle>
#include <QtGui/QApplication>


static QPixmap standardIcon(UserInputDialog::Icon icon, QWidget *widget)
{
    QStyle *style = widget ? widget->style() : QApplication::style();
    int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, 0, widget);
    QIcon tmpIcon;

    switch (icon)
    {
		case UserInputDialog::Information:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, 0, widget);
			break;

		case UserInputDialog::Warning:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, widget);
			break;

		case UserInputDialog::Critical:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, 0, widget);
			break;

		case UserInputDialog::Question:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, 0, widget);
			break;

		default:
			break;
    }

    if (tmpIcon.isNull())
        return QPixmap();
    else
        return tmpIcon.pixmap(iconSize, iconSize);
}


UserInputDialog::UserInputDialog(Icon icon, const QString &title, const QString &question, qint32 buttons, QWidget *parent) :
	QDialog(parent),
	m_answer(QDialogButtonBox::NoButton),
	m_layout(this),
	m_text(question),
	m_buttonBox(QDialogButtonBox::StandardButtons(buttons))
{
	setWindowTitle(title);

	m_vLayout.addWidget(&m_text);
	m_vLayout.addWidget(&m_lineEdit);

	m_iconLabel.setPixmap(standardIcon(icon, this));
	m_hLayout.addWidget(&m_iconLabel);
	m_hLayout.addLayout(&m_vLayout);

	m_layout.addLayout(&m_hLayout);
	m_layout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()),                this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()),                this, SLOT(reject()));
    connect(&m_buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void UserInputDialog::buttonClicked(QAbstractButton *button)
{
	m_answer = m_buttonBox.standardButton(button);
}
