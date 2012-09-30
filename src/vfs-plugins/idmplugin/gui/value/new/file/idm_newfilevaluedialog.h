#ifndef IDM_NEWFILEVALUEDIALOG_H_
#define IDM_NEWFILEVALUEDIALOG_H_

#include <QtCore/QStringList>
#include "../composite/idm_newcompositevaluedialog.h"


class NewFileValueDialog : public NewCompositeValueDialog
{
	Q_OBJECT

public:
	NewFileValueDialog(const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent = 0);

protected:
	virtual void addValue();
	virtual void removeValue();
};

#endif /* IDM_NEWFILEVALUEDIALOG_H_ */
