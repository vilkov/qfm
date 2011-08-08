#include "artifactscolumn.h"


ArtifactsColumn::ArtifactsColumn(const ArtifactInstance &instance) :
	m_instance(instance)
{}

ArtifactsColumn::ArtifactsColumn(const ArtifactsColumn &other) :
	m_instance(other.m_instance)
{}

QString ArtifactsColumn::data() const
{
	QString res;

	if (!m_instance.artifact()->isVirtual())
	{
		QList<qint32> list = m_instance.ids();

		if (!list.isEmpty())
		{
			res = m_instance.data<QVariant>(list[0]).toString();

			for (QList<qint32>::size_type i = 1; i < list.size(); ++i)
				res += ", " + m_instance.data<QVariant>(list[i]).toString();
		}
	}

	return res;
}
