#ifndef CHOOSEENTITYDIALOG_H_
#define CHOOSEENTITYDIALOG_H_

#include <QtCore/QList>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../containeres/idmcontainer.h"


using namespace FileSystem::Plugins::Idm;

class ChooseEntityDialog : public QDialog
{
	Q_OBJECT

public:
	typedef QList<IdmEntity*> List;

public:
	IdmEntity *value() const { return m_container.at(m_comboBox.itemData(m_comboBox.currentIndex()).toInt()); }
	static IdmEntity *chooseFile(const IdmContainer &container, QWidget *parent = 0);
	static IdmEntity *chooseProperty(const IdmContainer &container, IdmEntity *entity, QWidget *parent = 0);

protected:
	ChooseEntityDialog(const QString &title, const IdmContainer &container, const List &list, QWidget *parent = 0);

private:
	const IdmContainer &m_container;
	QLabel m_label;
	QComboBox m_comboBox;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
};

#endif /* CHOOSEENTITYDIALOG_H_ */
