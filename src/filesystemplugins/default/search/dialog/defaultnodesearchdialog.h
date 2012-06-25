#ifndef DEFAULTNODESEARCHDIALOG_H_
#define DEFAULTNODESEARCHDIALOG_H_

#include <QtGui/QDialog>
#include "../../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchDialog : public QDialog
{
public:
	SearchDialog(QWidget *parent = 0);
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTNODESEARCHDIALOG_H_ */
