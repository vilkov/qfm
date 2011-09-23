#ifndef CHOOSEFILEENTITYDIALOG_H_
#define CHOOSEFILEENTITYDIALOG_H_

#include <QtCore/QList>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../containeres/idmcontainer.h"


using namespace FileSystem::Plugins::Idm;

class ChooseFileEntityDialog : public QDialog
{
	Q_OBJECT

public:
	typedef QList<IdmEntity*> List;

public:
	ChooseFileEntityDialog(const IdmContainer &container, const List &list, QWidget *parent = 0);

	IdmEntity *value() const { return m_container.at(m_comboBox.itemData(m_comboBox.currentIndex()).toInt()); }
	static IdmEntity *choose(const IdmContainer &container, QWidget *parent = 0);

private:
	const IdmContainer &m_container;
	QLabel m_label;
	QComboBox m_comboBox;
	QDialogButtonBox m_buttonBox;
	QHBoxLayout m_horizontalLayout;
	QVBoxLayout m_verticatLayout;
};

#endif /* CHOOSEFILEENTITYDIALOG_H_ */
