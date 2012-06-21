#include "defaultpluginsettings.h"


DEFAULT_PLUGIN_NS_BEGIN

static const char plugin_id[] = "Folders";


Settings::Settings() :
	m_id(QByteArray::fromRawData(plugin_id, qstrlen(plugin_id))),
	m_name(tr("Folders"))
{}

const QByteArray &Settings::id() const
{
	return m_id;
}

const QString &Settings::name() const
{
	return m_name;
}

QWidget *Settings::centralWidget() const
{
	return NULL;//new QWidget();
}

DEFAULT_PLUGIN_NS_END
