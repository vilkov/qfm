#ifndef NEWCOMPOSITEVALUEDIALOG_H_
#define NEWCOMPOSITEVALUEDIALOG_H_

#include "../../composite/compositevaluedialog.h"


class NewCompositeValueDialog : public CompositeValueDialog
{
	Q_OBJECT

public:
	NewCompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent = 0);
};

#endif /* NEWCOMPOSITEVALUEDIALOG_H_ */
