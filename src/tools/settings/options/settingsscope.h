#ifndef SETTINGSSCOPE_H_
#define SETTINGSSCOPE_H_

#include "settingslist.h"


SETTINGS_NS_BEGIN

class Scope : public List
{
public:
	Scope(const Id &id, Option *parent) :
		List(id, parent)
	{}

	void manage(Option *option);

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();
};

SETTINGS_NS_END

#endif /* SETTINGSSCOPE_H_ */
