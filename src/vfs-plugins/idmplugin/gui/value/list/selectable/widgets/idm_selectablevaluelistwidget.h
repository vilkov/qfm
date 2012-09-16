#ifndef IDM_SELECTABLEVALUELISTWIDGET_H_
#define IDM_SELECTABLEVALUELISTWIDGET_H_

#include "../../editable/widgets/idm_editablevaluelistwidget.h"


class SelectableValueListWidget : public EditableValueListWidget
{
public:
	SelectableValueListWidget(const IdmContainer &container, const Select &query, NestedDialog *parent);

	virtual void accept();
};

#endif /* IDM_SELECTABLEVALUELISTWIDGET_H_ */
