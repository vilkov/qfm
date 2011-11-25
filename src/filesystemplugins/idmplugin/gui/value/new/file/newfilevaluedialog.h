#ifndef NEWFILEVALUEDIALOG_H_
#define NEWFILEVALUEDIALOG_H_

#include <QtCore/QStringList>
#include "../composite/newcompositevaluedialog.h"


class NewFileValueDialog : public NewCompositeValueDialog
{
	Q_OBJECT

public:
	NewFileValueDialog(const IdmContainer &container, IdmCompositeEntityValue *value, QWidget *parent = 0);

protected:
	virtual void addValue();
	virtual void removeValue();
};

#endif /* NEWFILEVALUEDIALOG_H_ */
