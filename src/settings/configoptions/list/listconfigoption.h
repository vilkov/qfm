#ifndef LISTCONFIGOPTION_H_
#define LISTCONFIGOPTION_H_

#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QScopedPointer>
#include <QtCore/QCoreApplication>
#include "model/listconfigoptionmodel.h"
#include "model/listconfigoptiondelegate.h"
#include "../../../tools/settings/configoptions/configoptionvaluedefault.h"


class ListConfigOption: public ConfigOptionValueDefault
{
	Q_DECLARE_TR_FUNCTIONS(ListConfigOption)

public:
	ListConfigOption(const qint32 &id, const QString &title, ConfigOption *parent);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

	void setListener(QObject *listener, const char *addSlot, const char *removeSlot);
	void add(const QString &value);
	void remove();

private:
	QWidget *m_widget;
	QTreeView *m_view;
	ListConfigOptionModel *m_model;
	QObject *m_listener;
	const char *m_addSlot;
	const char *m_removeSlot;
};

#endif /* LISTCONFIGOPTION_H_ */
