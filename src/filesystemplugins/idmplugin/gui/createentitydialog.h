#ifndef CREATEENTITYDIALOG_H_
#define CREATEENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QDialogButtonBox>
#include "../items/idmitemslist.h"
#include "../containeres/idmentitytypes.h"


class CreateEntityDialog : public QDialog
{
    Q_OBJECT

public:
    CreateEntityDialog(const FileSystem::IdmItemsList *entities, const FileSystem::IdmEntityTypes &entityTypes, const QString &name = QString(), QWidget *parent = 0);

    QString name() const { return m_lineEdit.text().simplified(); }
    virtual void accept();

private:
    QLabel m_label;
    QLineEdit m_lineEdit;
    QLabel m_label2;
    QComboBox m_comboBox;
    QGridLayout m_gridLayout;
    QDialogButtonBox m_buttonBox;
};

#endif /* CREATEENTITYDIALOG_H_ */
