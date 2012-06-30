#ifndef DEFAULTSEARCHDIALOG_H_
#define DEFAULTSEARCHDIALOG_H_

#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchDialog : public QDialog
{
public:
	SearchDialog(QWidget *parent = 0);

    virtual void accept();
    virtual void reject();

private:
	QVBoxLayout m_layout;
	QHBoxLayout m_hLayout;
	QLabel m_label;
	QLineEdit m_editor;
	QDialogButtonBox m_buttonBox;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSEARCHDIALOG_H_ */
