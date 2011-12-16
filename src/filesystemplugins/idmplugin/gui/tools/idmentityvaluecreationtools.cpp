#include "idmentityvaluecreationtools.h"
#include "../value/new/composite/newcompositevaluedialog.h"
#include "../value/list/selectable/selectablevaluelistdialog.h"
#include "../../../../tools/pointers/pscopedpointer.h"
#include "../../../../tools/widgets/valuedialog/valuedialog.h"


IDM_PLUGIN_NS_BEGIN

template <Database::EntityType EntityType>
class NewValueDialog : public ValueDialog<typename EntityValueType<EntityType>::type>
{
public:
	typedef typename EntityValueType<EntityType>::type value_type;
	typedef ValueDialog<value_type>                    base_class;

public:
	NewValueDialog(const QString &title, const QString &label, QWidget *parent = 0) :
		base_class(title, label, value_type(), parent)
	{}
};


template <Database::EntityType EntityType>
inline IdmEntityValue *processAddValue(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	typedef NewValueDialog<EntityType> NewValueDialog;
	NewValueDialog dialog(title, label, parent);

	if (dialog.exec() == NewValueDialog::Accepted)
		return container.addValue(entity, dialog.value());
	else
		declined = true;

	return 0;
}

template <>
inline IdmEntityValue *processAddValue<Database::Memo>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	declined = false;
	return 0;
}

template <>
inline IdmEntityValue *processAddValue<Database::Composite>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	QByteArray name = Database::savepoint("processAddValue<Database::Composite>::");

	if (container.savepoint(name))
	{
		PScopedPointer<IdmCompositeEntityValue> value(container.addValue(entity));

		if (value)
		{
			NewCompositeValueDialog dialog(container, value.data(), parent);

			if (dialog.exec() == NewCompositeValueDialog::Accepted)
				if (container.release(name))
					return value.take();
				else
					container.rollback(name);
			else
			{
				declined = true;
				container.rollback(name);
			}
		}
	}

	return 0;
}

template <>
inline IdmEntityValue *processAddValue<Database::Rating>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	declined = false;
	return 0;
}

template <>
inline IdmEntityValue *processAddValue<Database::Path>(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	declined = false;
	return 0;
}


IdmEntityValue *CreationTools::createValue(const QString &title, const QString &label, QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	switch (entity->type())
	{
		case Database::Int:
			return processAddValue<Database::Int>(title, label, parent, container, entity, declined);

		case Database::String:
			return processAddValue<Database::String>(title, label, parent, container, entity, declined);

		case Database::Date:
			return processAddValue<Database::Date>(title, label, parent, container, entity, declined);

		case Database::Time:
			return processAddValue<Database::Time>(title, label, parent, container, entity, declined);

		case Database::DateTime:
			return processAddValue<Database::DateTime>(title, label, parent, container, entity, declined);

		case Database::Memo:
			return processAddValue<Database::Memo>(title, label, parent, container, entity, declined);

		case Database::Composite:
			return processAddValue<Database::Composite>(title, label, parent, container, entity, declined);

		case Database::Rating:
			return processAddValue<Database::Rating>(title, label, parent, container, entity, declined);

		case Database::Path:
			return processAddValue<Database::Path>(title, label, parent, container, entity, declined);

		default:
			return 0;
	}
}

IdmEntityValue *CreationTools::chooseOrCreateValue(QWidget *parent, IdmContainer &container, IdmEntity *entity, bool &declined)
{
	SelectableValueListDialog dialog(container, Select(entity), parent);

	if (dialog.exec() == EditableValueListDialog::Accepted)
		return dialog.takeValue();
	else
		declined = true;

	return 0;
}

IDM_PLUGIN_NS_END
