#ifndef FILESYSTEMMODELADAPTOR_H_
#define FILESYSTEMMODELADAPTOR_H_

#include "filesystemproxymodel.h"
#include "../../../tools/metatemplates.h"


class FileSystemModelAdaptor : public Templates::Adaptors::StrictBaseContainer<QModelIndexList, Templates::bool_value<false> >
{
public:
	typedef Templates::Adaptors::StrictBaseContainer<QModelIndexList, Templates::bool_value<false> > parent_class;

public:
	FileSystemModelAdaptor(const FileSystemProxyModel &proxy, const QModelIndexList &container) :
		parent_class(container),
		m_proxy(proxy)
	{}

	inline value_type at(size_type i) const { return m_proxy.mapToSource(parent_class::m_container.at(i)); }
	inline value_type operator[](size_type i) const { return m_proxy.mapToSource(parent_class::m_container[i]); }

private:
	const FileSystemProxyModel &m_proxy;
};

#endif /* FILESYSTEMMODELADAPTOR_H_ */
