#ifndef KDE_DE_P_H_
#define KDE_DE_P_H_

#include <QtCore/QString>


class DesktopEnvironmentPrivate
{
public:
	DesktopEnvironmentPrivate();

    static QString iconThemeName(int version);

private:
    static QString kdeHome(int version);
};

#endif /* KDE_DE_P_H_ */
