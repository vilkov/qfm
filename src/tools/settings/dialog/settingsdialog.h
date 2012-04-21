#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QFont>
#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "../configoptions/configoptiongroup.h"
#include "../constraints/configoptionconstraint.h"


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
