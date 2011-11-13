#ifndef NEWCOMPOSITEVALUEDIALOG_H_
#define NEWCOMPOSITEVALUEDIALOG_H_

#include "../../composite/compositevaluedialog.h"


class NewCompositeValueDialog : public CompositeValueDialog
{
	Q_OBJECT

public:
	NewCompositeValueDialog(const IdmContainer &container, IdmCompositeEntityValue *value, QWidget *parent = 0);

//    IdmEntityValue *value();

private:
	IdmEntity *m_entity;
};

#endif /* NEWCOMPOSITEVALUEDIALOG_H_ */
