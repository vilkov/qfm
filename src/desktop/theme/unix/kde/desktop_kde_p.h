#ifndef DESKTOP_KDE_P_H_
#define DESKTOP_KDE_P_H_

#include <QtCore/QString>


class DesktopPrivate
{
public:
	DesktopPrivate();

    static QString iconThemeName(int version);

private:
    static QString kdeHome(int version);
};

#endif /* DESKTOP_KDE_P_H_ */
