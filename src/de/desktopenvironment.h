#ifndef DESKTOPENVIRONMENT_H_
#define DESKTOPENVIRONMENT_H_

#include <QtCore/QString>


class DesktopEnvironment
{
public:
	enum Type
	{
		DE_Cde,
		DE_Kde,
		DE_4Dwm,
		DE_Gnome,
		DE_Unknown
	};

public:
	DesktopEnvironment();
	~DesktopEnvironment();

	Type type() const { return m_type; }
	QString themeName() const;

	bool info(const QString &absoluteFilePath) const;

private:
	Type m_type;
	int m_version;
};

#endif /* DESKTOPENVIRONMENT_H_ */
