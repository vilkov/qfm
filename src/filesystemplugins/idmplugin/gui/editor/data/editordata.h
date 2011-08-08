#ifndef EDITORDATA_H_
#define EDITORDATA_H_

#include "../../../artifactmanager.h"
#include "../../../artifact/artifact.h"
#include "../../../instance/artifactinstance.h"
#include "../../../data/artifactsdata.h"


class EditorData
{
public:
	enum EditorType
	{
		Master,
		Secondary
	};
	struct Parent
	{
		Artifact *artifact;
		qint32 id;
	};

public:
	EditorData(const ArtifactsData &data, const ArtifactInstance &reference, const EditorType type = Master) :
		m_type(type),
		m_instance(reference),
		m_data(data)
	{}
	EditorData(const EditorData &other) :
		m_type(Secondary),
		m_instance(other.m_instance),
		m_data(other.m_data)
	{}

	EditorType type() const { return m_type; }
	ArtifactManager &manager() const { return m_data.manager(); }
	ArtifactInstance &instance() { return m_instance; }
	ArtifactsData &data() { return m_data; }
	const Parent &parent() const { return m_parent; }
	void setParent(Artifact *artifact, const qint32 &id) { m_parent.artifact = artifact; m_parent.id = id; }

private:
	EditorType m_type;
	ArtifactInstance m_instance;
	ArtifactsData m_data;
	Parent m_parent;
};

#endif /* EDITORDATA_H_ */
