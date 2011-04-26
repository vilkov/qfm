#ifndef CONTEXTMENU_H_
#define CONTEXTMENU_H_

#include <QtCore/QSharedData>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtGui/QWidget>


class ContextMenu
{
public:
	class Implementation : public QSharedData
	{
		Q_DISABLE_COPY(Implementation)

	public:
		Implementation(QWidget *parent) :
			QSharedData(),
			m_parent(parent)
		{}

		QWidget *parent() const { return m_parent; }
		virtual void popup(const QString &parentDir, const QStringList &files, const QPoint &pos) = 0;

	private:
		QWidget *m_parent;
	};

public:
	ContextMenu(QWidget *parent);

	void popup(const QString &parentDir, const QStringList &files);

private:
    QExplicitlySharedDataPointer<Implementation> m_data;
};

#endif /* CONTEXTMENU_H_ */
