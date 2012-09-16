#ifndef IDM_CHOOSEENTITYDIALOG_H_
#define IDM_CHOOSEENTITYDIALOG_H_

#include <QtCore/QList>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../containeres/idm_container.h"


using namespace ::VFS::Plugins::Idm;

class ChooseEntityDialog : public QDialog
{
	Q_OBJECT

public:
	typedef QList<IdmEntity*> List;

public:
	IdmEntity *value() const { return m_container.at(m_comboBox.itemData(m_comboBox.currentIndex()).toInt()); }
	static IdmEntity *chooseFile(const IdmContainer &container, QWidget *parent = 0);
	static IdmEntity *chooseProperty(const IdmContainer &container, IdmEntity *entity, QString &propertyName, QWidget *parent = 0);

protected:
	ChooseEntityDialog(const QString &title, const IdmContainer &container, const List &list, QWidget *parent = 0);

protected:
	const IdmContainer &m_container;
	QVBoxLayout m_verticatLayout;

private:
	QLabel m_label;
	QComboBox m_comboBox;
	QDialogButtonBox m_buttonBox;
	QHBoxLayout m_horizontalLayout;
};

#endif /* IDM_CHOOSEENTITYDIALOG_H_ */
