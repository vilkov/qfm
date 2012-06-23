#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtCore/QMap>
#include <QtGui/QFont>
#include <QtGui/QDialog>
#include <QtGui/QSplitter>
#include <QtGui/QTreeView>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../configoptions/configoptiongroup.h"
#include "../constraints/configoptionconstraint.h"


#include "model/settingsdialogmodel.h"
#include "settings/settingsdialogsettings.h"


using namespace Tools::Settings;


class Dialog : public QDialog
{
public:
	typedef Model::Pages            Pages;
	typedef QMap<Page *, QLayout *> PagesCache;

public:
	Dialog(const QString &title, DialogSettings &settings, const Pages &pages, QWidget *parent = 0);
	virtual ~Dialog();

public:
    virtual void accept();
    virtual void reject();

private:
	DialogSettings &m_settings;
	QVBoxLayout m_layout;
	QSplitter m_splitter;
	QTreeView m_pagesView;
	QGroupBox m_selectedPage;
	QDialogButtonBox m_buttonBox;

private:
	Model m_model;
	PagesCache m_editedPages;
};



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
