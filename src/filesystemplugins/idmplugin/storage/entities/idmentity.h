#ifndef IDMENTITY_H_
#define IDMENTITY_H_

#include <QtCore/QString>
#include "../../../../filesystem/filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntity
{
public:
	enum Type
	{
		Int        = 1,
		String     = 2,
		Date       = 3,
		Time       = 4,
		DateTime   = 5,
		Memo       = 6,
		Composite  = 7,
		Raiting    = 8
	};
	typedef unsigned int id_type;

public:
	IdmEntity(id_type id, const QString &name) :
		m_id(id),
		m_name(name)
	{}
	virtual ~IdmEntity()
	{}

	id_type id() const { return m_id; }
	const QString &name() const { return m_name; }

	virtual Type type() const = 0;

private:
	id_type m_id;
	QString m_name;
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITY_H_ */
