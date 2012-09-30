#ifndef IDM_NEWCOMPOSITEVALUEDIALOG_H_
#define IDM_NEWCOMPOSITEVALUEDIALOG_H_

#include "../../composite/idm_compositevaluedialog.h"


class NewCompositeValueDialog : public CompositeValueDialog
{
	Q_OBJECT

public:
	NewCompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, QWidget *parent = 0);
	NewCompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent = 0);
};

#endif /* IDM_NEWCOMPOSITEVALUEDIALOG_H_ */
