#ifndef COMPOSITEVALUEWIDGET_H_
#define COMPOSITEVALUEWIDGET_H_

#include <QtGui/QTreeView>
#include "../../model/compositevaluemodel.h"
#include "../../../../containeres/idmcontainer.h"
#include "../../../../../../tools/events/imp/mouseeventhandler.h"
#include "../../../../../../tools/events/imp/mouseeventsource.h"
#include "../../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../../tools/events/imp/keyboardeventsource.h"
#include "../../../../../../tools/widgets/nestedplaindialog/widgets/nestedwidget.h"


using namespace FileSystem::Plugins::Idm;

class CompositeValueWidgetPrivate
{
public:
	class ICallback
	{
	public:
		virtual ~ICallback();

		virtual NestedDialog *parent() = 0;
		virtual void critical(const QString &text) = 0;
	};

	typedef MouseDoubleClickEventSource<
				KeyboardEventSource<
					EventSourceBase<
						QTreeView
					>
				>
			> TreeView;

public:
	CompositeValueWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value);
	CompositeValueWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value, const CompositeValueModel::Files &files);

	const IdmContainer &container() const { return m_container; }
	IdmContainer &container() { return m_container; }

	const TreeView &view() const { return m_view; }
	TreeView &view() { return m_view; }

	const CompositeValueModel &model() const { return m_model; }
	CompositeValueModel &model() { return m_model; }

	void open(const QModelIndex &index);
	void addValue(const QModelIndex &index);
	void removeValue(const QModelIndex &index);

private:
	ICallback *m_callback;
	IdmContainer m_container;
	IdmEntityValue::Holder m_value;
	TreeView m_view;
	CompositeValueModel m_model;
};


class MainCompositeValueWidget : public BaseNestedWidget, public CompositeValueWidgetPrivate::ICallback
{
public:
	MainCompositeValueWidget(EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value, NestedDialog *parent);
	MainCompositeValueWidget(EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value, const CompositeValueModel::Files &files, NestedDialog *parent);

	/* BaseNestedWidget */
	virtual QWidget *centralWidget();
	virtual void setReadOnly(bool value);
	virtual void setFocus();

	/* CompositeValueWidgetPrivate::ICallback */
	virtual NestedDialog *parent();
	virtual void critical(const QString &text);

    QModelIndex currentIndex() const { return m_private.view().selectionModel()->currentIndex(); }

    const IdmContainer &container() const { return m_private.container(); }
	IdmContainer &container() { return m_private.container(); }

	const CompositeValueModel &model() const { return m_private.model(); }
	CompositeValueModel &model() { return m_private.model(); }

	void open(const QModelIndex &index) { m_private.open(index); }
	void addValue(const QModelIndex &index) { m_private.addValue(index); }
	void removeValue(const QModelIndex &index) { m_private.removeValue(index); }

private:
	CompositeValueWidgetPrivate m_private;
};


class CompositeValueWidget : public NestedWidget, public CompositeValueWidgetPrivate::ICallback
{
public:
	CompositeValueWidget(const IdmContainer &container, const IdmEntityValue::Holder &value, NestedDialog *parent, const QString &title);

	/* BaseNestedWidget */
	virtual void setFocus();

	/* CompositeValueWidgetPrivate::ICallback */
	virtual NestedDialog *parent();
	virtual void critical(const QString &text);

    QModelIndex currentIndex() const { return m_private.view().selectionModel()->currentIndex(); }

    const IdmContainer &container() const { return m_private.container(); }
	IdmContainer &container() { return m_private.container(); }

	const CompositeValueModel &model() const { return m_private.model(); }
	CompositeValueModel &model() { return m_private.model(); }

	void addValue();
	void removeValue();

private:
	typedef KeyboardEventHandler<
				EventHandlerBase<
					CompositeValueWidget
				>
			> TreeViewHandler;

private:
	TreeViewHandler m_handler;
	CompositeValueWidgetPrivate m_private;
};

#endif /* COMPOSITEVALUEWIDGET_H_ */
