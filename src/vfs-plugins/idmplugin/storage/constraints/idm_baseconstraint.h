#ifndef IDM_BASECONSTRAINT_H_
#define IDM_BASECONSTRAINT_H_

#include <QtCore/QSharedData>
#include "../entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class BaseConstraint : public QSharedData
{
public:
	typedef QExplicitlySharedDataPointer<BaseConstraint> Holder;

public:
	BaseConstraint(BaseConstraint *parent = 0);
	virtual ~BaseConstraint();

	BaseConstraint *parent() const { return m_parent; }

	virtual bool isGroup() const = 0;
	virtual QString toString() const = 0;

private:
	BaseConstraint *m_parent;
};

IDM_PLUGIN_NS_END

#endif /* IDM_BASECONSTRAINT_H_ */
