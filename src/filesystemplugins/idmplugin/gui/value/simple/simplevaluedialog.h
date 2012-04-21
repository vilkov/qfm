#ifndef SIMPLEVALUEDIALOG_H_
#define SIMPLEVALUEDIALOG_H_

#include "widgets/simplevaluewidget.h"
#include "../../../storage/structure/idmdatabasetypes.h"
#include "../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


using namespace FileSystem::Plugins::Idm;

template <Database::EntityType EntityType>
class SimpleValueDialog : public NestedPlainDialog
{
public:
	typedef typename EntityValueType<EntityType>::type value_type;
	typedef ValueDialog<value_type>                    base_class;

public:
	SimpleValueDialog(const QString &title, QWidget *parent = 0) :
		NestedPlainDialog(parent)
	{
		setWindowTitle(title);
		setCentralWidget(&m_mainWidget);
	}

	value_type value() const { return m_mainWidget.value(); }

private:
	MainSimpleValueWidget<value_type> m_mainWidget;
};

#endif /* SIMPLEVALUEDIALOG_H_ */
