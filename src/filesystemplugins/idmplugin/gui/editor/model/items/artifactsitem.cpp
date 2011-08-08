#include "artifactsitem.h"


ArtifactsItem::ArtifactsItem(const qint32 &id, ModelItem *parent) :
	ModelItem(parent),
	m_id(id)
{}

ArtifactsItem::~ArtifactsItem()
{
	clear();
}
