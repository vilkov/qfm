#include "idm_entity.h"


IDM_PLUGIN_NS_BEGIN

Entity::Entity(Type type, id_type id,
		const QString &name,
		const ShortFormat &shortFormat,
		const QByteArray &editorGeometry,
		const QByteArray &listGeometry) :
	m_type(type),
	m_id(id),
	m_name(name),
	m_shortFormat(shortFormat),
	m_editorGeometry(editorGeometry.isEmpty() ? QRect() : geometryFromByteArray(editorGeometry)),
	m_listGeometry(listGeometry.isEmpty() ? QRect() : geometryFromByteArray(listGeometry))
{}

Entity::~Entity()
{}

QByteArray Entity::geometryToByteArray(const QRect &geometry)
{
	QByteArray buffer(sizeof(int) * 4, Qt::Uninitialized);
	int *data = (int *)buffer.data();

	data[0] = geometry.left();
	data[1] = geometry.top();
	data[2] = geometry.right();
	data[3] = geometry.bottom();

	return buffer;
}

QRect Entity::geometryFromByteArray(const QByteArray &buffer)
{
	const int *data = (const int *)buffer.data();
	return QRect(QPoint(data[0], data[1]), QPoint(data[2], data[3]));
}

IDM_PLUGIN_NS_END
