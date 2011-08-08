#ifndef ARTIFACTSCOLUMN_H_
#define ARTIFACTSCOLUMN_H_

#include <QList>
#include <QString>
#include <QVariant>
#include "../../../../instance/artifactinstance.h"


class ArtifactsColumn
{
public:
	ArtifactsColumn(const ArtifactInstance &instance);
	ArtifactsColumn(const ArtifactsColumn &other);

	QString data() const;
	const ArtifactInstance &instance() const { return m_instance; }

private:
	ArtifactInstance m_instance;
};

#endif /* ARTIFACTSCOLUMN_H_ */
