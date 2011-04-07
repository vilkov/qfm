#ifndef ICONPROVIDER_H_
#define ICONPROVIDER_H_

#include <QtCore/QMutex>
#include <QtGui/QFileIconProvider>


class IconProvider
{
public:
	IconProvider() :
		m_iconProvider(&m_defaultProvider)
	{}

	void lock() { m_lock.lock(); }
    QFileIconProvider *iconProvider() const { return m_iconProvider; }
    void setIconProvider(QFileIconProvider *provider);
	void unlock() { m_lock.unlock(); }

private:
    QMutex m_lock;
    QFileIconProvider m_defaultProvider;
    QFileIconProvider *m_iconProvider;
};

#endif /* ICONPROVIDER_H_ */
