#ifndef EDITCOMPOSITEVALUEDIALOG_H_
#define EDITCOMPOSITEVALUEDIALOG_H_

#include "../composite/compositevaluedialog.h"


class EditCompositeValueDialog : public CompositeValueDialog
{
	Q_OBJECT

public:
	EditCompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent = 0);
};

#endif /* EDITCOMPOSITEVALUEDIALOG_H_ */
