#ifndef CREATEENTITYDIALOG_H_
#define CREATEENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/entitieslistmodel.h"
#include "model/entitieslistdelegate.h"


using namespace FileSystem::Plugins::Idm;

class CreateEntityDialog : public QDialog
{
    Q_OBJECT

public:
	typedef EntitiesListModel::size_type size_type;

public:
    CreateEntityDialog(const IdmContainer &container, const QString &name = QString(), QWidget *parent = 0);

    IdmEntity::Type type() const { return static_cast<IdmEntity::Type>(m_comboBox.itemData(m_comboBox.currentIndex(), Qt::UserRole).toInt()); }
    QString name() const { return m_lineEdit.text().simplified(); }
    IdmShortFormat shortFormat() const { return m_lineEdit2.text().simplified(); }

	IdmEntity *property(size_type index) const { return m_model.entityAt(index); }
	const QString &propertyName(size_type index) const { return m_model.nameAt(index); }
	size_type size() const { return m_model.size(); }

    virtual void accept();

private Q_SLOTS:
	void activated(int index);
	void add();
	void remove();

private:
	void setListEnabled(bool enabled);

private:
    QLabel m_label;
    QLineEdit m_lineEdit;
    QLabel m_label2;
    QLineEdit m_lineEdit2;
    QLabel m_label3;
    QComboBox m_comboBox;
    QTreeView m_view;
    QPushButton m_addEntity;
    QPushButton m_removeEntity;
    QGridLayout m_gridLayout;
    QGridLayout m_gridLayout2;
    QHBoxLayout m_horizontalLayout;
    QDialogButtonBox m_buttonBox;
    EntitiesListModel m_model;
    EntitiesListDelegate m_delegate;
};

#endif /* CREATEENTITYDIALOG_H_ */
