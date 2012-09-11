#ifndef SETTINGSOPTIONSLIST_H_
#define SETTINGSOPTIONSLIST_H_

#include "settingslist.h"


SETTINGS_NS_BEGIN

class OptionList : public List
{
public:
	OptionList(const Id &id, Option *parent) :
		List(id, parent)
	{}
	virtual ~OptionList();

	void add(Option *option) { m_items.push_back(&option->id(), option); }
	void remove(const Id &id) { delete m_items.take(&id); }
	void clear();

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
	virtual bool isSubOptionName(const QStringRef &name) const = 0;
	virtual Option *create() = 0;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONSLIST_H_ */
