#include "idmentity.h"


IDM_PLUGIN_NS_BEGIN

IdmEntity::IdmEntity(Type type, id_type id,
		const QString &name,
		const IdmShortFormat &shortFormat,
		const QByteArray &editorGeometry,
		const QByteArray &listGeometry) :
	m_type(type),
	m_id(id),
	m_name(name),
	m_shortFormat(shortFormat)
{}

IdmEntity::~IdmEntity()
{}

QByteArray IdmEntity::geometryToByteArray(const QRect &geometry)
{
	QByteArray buffer(sizeof(int) * 4, Qt::Uninitialized);
	int *data = (int *)buffer.data();

	data[0] = geometry.left();
	data[1] = geometry.top();
	data[2] = geometry.right();
	data[3] = geometry.bottom();

	return buffer;
}

QRect IdmEntity::geometryfromByteArray(const QByteArray &buffer)
{
	const int *data = (const int *)buffer.data();
	return QRect(QPoint(data[0], data[1]), QPoint(data[2], data[3]));
}

IDM_PLUGIN_NS_END
