#ifndef IDMMENU_H_
#define IDMMENU_H_

#include "idmmenuitem.h"
#include "../../items/idmitemslist.h"


FILE_SYSTEM_NS_BEGIN

class IdmMenu : public IdmItemsList
{
public:
	IdmMenu(const QString &label, const QString &toolTip, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	IdmMenu *add(const QString &label, const QString &toolTip)
	{
		IdmMenu *res;
		m_items.push_back(res = new IdmMenu(label, toolTip, this));
		return res;
	}
	void add(IdmMenuItem::id_type id, const QString &label, const QString &toolTip)
	{
		m_items.push_back(new IdmMenuItem(id, label, toolTip, this));
	}
	void add(IdmItem *item)
	{
		m_items.push_back(item);
	}

private:
	QVariant m_label;
	QVariant m_toolTip;
};

FILE_SYSTEM_NS_END

#endif /* IDMMENU_H_ */
