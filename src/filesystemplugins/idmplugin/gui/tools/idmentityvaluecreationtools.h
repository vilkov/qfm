#ifndef IDMENTITYVALUECREATIONTOOLS_H_
#define IDMENTITYVALUECREATIONTOOLS_H_

#include "nestedplaindialog/widgets/nesteddialog.h"
#include "../../containeres/idmcontainer.h"
#include "../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

struct CreationTools
{
	static IdmEntityValue::Holder createValue(NestedDialog *parent, const QString &title, const QString &label, IdmContainer &container, IdmEntity *entity, bool &declined);
	static IdmEntityValue::Holder chooseOrCreateValue(QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined);
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYVALUECREATIONTOOLS_H_ */
