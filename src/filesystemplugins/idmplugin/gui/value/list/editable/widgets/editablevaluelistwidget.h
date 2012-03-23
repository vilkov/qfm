#ifndef EDITABLEVALUELISTWIDGET_H_
#define EDITABLEVALUELISTWIDGET_H_

#include <QtGui/QTreeView>
#include "../model/editablevaluelistmodel.h"
#include "../../../../../containeres/idmcontainer.h"
#include "../../../../../storage/queries/idmselectquery.h"
#include "../../../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../../../tools/events/imp/keyboardeventsource.h"
#include "../../../../../../../tools/widgets/nestedplaindialog/widgets/nestedwidget.h"


using namespace FileSystem::Plugins::Idm;

class EditableValueListWidgetPrivate
{
public:
	class ICallback
	{
	public:
		virtual ~ICallback();

		virtual NestedDialog *parent() = 0;
		virtual void critical(const QString &text) = 0;
	};

	typedef KeyboardEventSource<
				EventSourceBase<
					QTreeView
				>
			> TreeView;

public:
	EditableValueListWidgetPrivate(ICallback *callback, EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value);

	const IdmContainer &container() const { return m_container; }
	IdmContainer &container() { return m_container; }

	const TreeView &view() const { return m_view; }
	TreeView &view() { return m_view; }

	const EditableValueListModel &model() const { return m_model; }
	EditableValueListModel &model() { return m_model; }

private:
	void addValue();
	void removeValue();
    void setCurrentIndex(const QModelIndex &index) const;
    void select(const QModelIndex &index);

private:
	ICallback *m_callback;
	IdmContainer m_container;
	IdmEntity *m_entity;
	TreeView m_view;
	EditableValueListModel m_model;
};


class MainEditableValueListWidget : public BaseNestedWidget, public CompositeValueWidgetPrivate::ICallback
{
public:
	MainEditableValueListWidget(EventHandler *handler, const IdmContainer &container, const IdmEntityValue::Holder &value, NestedDialog *parent);

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

	void addValue(const QModelIndex &index) { m_private.addValue(index); }
	void removeValue(const QModelIndex &index) { m_private.removeValue(index); }

private:
	EditableValueListWidgetPrivate m_private;
};


class EditableValueListWidget : public NestedWidget, public CompositeValueWidgetPrivate::ICallback
{
public:
	EditableValueListWidget(const IdmContainer &container, const IdmEntityValue::Holder &value, NestedDialog *parent, const QString &title);

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
	EditableValueListWidgetPrivate m_private;
};

#endif /* EDITABLEVALUELISTWIDGET_H_ */
