#ifndef CONFIGOPTIONCONSTRAINTS_H_
#define CONFIGOPTIONCONSTRAINTS_H_

#include <QWidget>
#include <QVariant>


class ConfigOptionConstraint
{
public:
	virtual ~ConfigOptionConstraint() {}
	virtual bool check(QWidget *parent) const = 0;
};

#endif /* CONFIGOPTIONCONSTRAINTS_H_ */
