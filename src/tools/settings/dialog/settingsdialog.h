#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QFont>
#include <QtGui/QDialog>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../configoptions/configoptiongroup.h"
#include "../constraints/configoptionconstraint.h"


using namespace ::Tools::Settings;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    typedef QList<ConfigOptionGroup*>      SettingsList;
    typedef QList<ConfigOptionConstraint*> ConstraintsList;

public:
    SettingsDialog(const QString &title, const SettingsList &settings, const ConstraintsList &constraints, QWidget *parent = 0);

    virtual void accept();

private:
    QFont createFont() const;

private:
    QTabWidget m_tabs;
    QVBoxLayout m_layout;
	QDialogButtonBox m_buttonBox;
	ConstraintsList m_constraints;
};

#endif // SETTINGSDIALOG_H
