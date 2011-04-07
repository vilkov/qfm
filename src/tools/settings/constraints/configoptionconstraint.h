#ifndef CONFIGOPTIONCONSTRAINTS_H_
#define CONFIGOPTIONCONSTRAINTS_H_

#include <QWidget>
#include <QVariant>
#include "../../memory/memory_manager.h"


class ConfigOptionConstraint : public MemoryManagerTag
{
public:
	virtual ~ConfigOptionConstraint() {}
	virtual bool check(QWidget *parent) const = 0;
};

#endif /* CONFIGOPTIONCONSTRAINTS_H_ */
