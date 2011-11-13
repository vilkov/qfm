#ifndef IDMENTITYVALUECREATIONTOOLS_H_
#define IDMENTITYVALUECREATIONTOOLS_H_

#include "../../containeres/idmcontainer.h"
#include "../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

struct CreationTools
{
	static IdmEntityValue *createValue(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined);
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYVALUECREATIONTOOLS_H_ */
