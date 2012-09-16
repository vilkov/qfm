#ifndef IDM_EDITCOMPOSITEVALUEDIALOG_H_
#define IDM_EDITCOMPOSITEVALUEDIALOG_H_

#include "../composite/idm_compositevaluedialog.h"


class EditCompositeValueDialog : public CompositeValueDialog
{
	Q_OBJECT

public:
	EditCompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent = 0);
};

#endif /* IDM_EDITCOMPOSITEVALUEDIALOG_H_ */
