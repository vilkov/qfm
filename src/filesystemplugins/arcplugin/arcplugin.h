#ifndef ARCPLUGIN_H_
#define ARCPLUGIN_H_

#include "actions/arcpackaction.h"
#include "actions/arcunpackaction.h"
#include "actions/arcunpackhereaction.h"
#include "actions/arcunpackintosubdiraction.h"
#include "plugins/arclibarchiveplugin.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


ARC_PLUGIN_NS_BEGIN

class Plugin : public IFileReaderPlugin
{
public:
	Plugin();

	/* ISettings */
	virtual void beginGroup(const QString &name);
	virtual void writeValue(const QString &name, const QVariant &value);
	virtual QVariant readValue(const QString &name, const QVariant &defaultValue = QVariant());
	virtual void endGroup();

	/* IGlobalSettings */
	virtual QString id() const;

	virtual void registered();
	virtual Node *node(const IFileContainer *container, const IFileInfo *file, Node *parent) const;
	virtual FileTypeIdList fileTypes() const;

	static const Archive **archivers();

private:
	PackAction m_packAction;
	UnPackAction m_unPackAction;
	UnPackHereAction m_unPackHereAction;
	UnPackIntoSubdirAction m_unPackIntoSubdirAction;
	LibArchivePlugin m_libArchive;
	const Archive *m_archivers[2];
};

ARC_PLUGIN_NS_END

#endif /* ARCPLUGIN_H_ */
