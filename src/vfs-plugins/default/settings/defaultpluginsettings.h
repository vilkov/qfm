#ifndef DEFAULTPLUGINSETTINGS_H_
#define DEFAULTPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../tools/settings/options/gui/page/settingspage.h"
#include "../../../tools/settings/options/gui/decoration/settingsdecorationtext.h"
#include "../../../tools/settings/options/gui/groupbox/settingsgroupbox.h"
#include "../../../tools/settings/options/gui/radiobutton/settingsradiobutton.h"


DEFAULT_PLUGIN_NS_BEGIN

class Settings : public ::Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	class FileIcon : public ::Tools::Settings::GroupBox
	{
		Q_DECLARE_TR_FUNCTIONS(FileIcon)

	public:
		FileIcon(Option *parent);

		const ::Tools::Settings::RadioButton &onlyAppIcon() const { return m_onlyAppIcon; }
		const ::Tools::Settings::RadioButton &onlyTypeIcon() const { return m_onlyTypeIcon; }
		const ::Tools::Settings::RadioButton &appIconIfNoTypeIcon() const { return m_appIconIfNoTypeIcon; }

	private:
		::Tools::Settings::RadioButton m_onlyAppIcon;
		::Tools::Settings::RadioButton m_onlyTypeIcon;
		::Tools::Settings::RadioButton m_appIconIfNoTypeIcon;
	};

public:
	Settings(Option *parent);

	static Settings *instance();

	const FileIcon &fileIcon() const { return m_fileIcon; }

private:
	::Tools::Settings::DecorationText m_text;
	FileIcon m_fileIcon;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPLUGINSETTINGS_H_ */
