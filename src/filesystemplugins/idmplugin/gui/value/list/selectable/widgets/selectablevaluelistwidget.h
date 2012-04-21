#ifndef SELECTABLEVALUELISTWIDGET_H_
#define SELECTABLEVALUELISTWIDGET_H_

#include "../../editable/widgets/editablevaluelistwidget.h"


class SelectableValueListWidget : public EditableValueListWidget
{
public:
	SelectableValueListWidget(const IdmContainer &container, const Select &query, NestedDialog *parent);

	virtual void accept();
};

#endif /* SELECTABLEVALUELISTWIDGET_H_ */
