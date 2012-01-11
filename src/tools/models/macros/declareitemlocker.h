#ifndef DECLAREITEMLOCKER_H_
#define DECLAREITEMLOCKER_H_

#include <QtCore/QString>
#include <QtCore/QModelIndex>
#include <QtGui/QIcon>
#include "../models_ns.h"


MODELS_NS_BEGIN

struct ModelItemLocker
{
	ModelItemLocker() :
		m_locked(false)
	{}

	bool isLocked() const { return m_locked; }
	const QString &lockReason() const { return m_reason; }
	const QIcon &lockIcon() const { return m_icon; }

	void lock(const QString &reason) { m_reason = reason; m_locked = true; }
	void lock(const QString &reason, const QIcon &icon) { m_reason = reason; m_icon = icon; m_locked = true; }
	void unlock() { m_locked = false; m_reason.clear(); }

private:
	bool m_locked;
	QString m_reason;
	QIcon m_icon;
};

MODELS_NS_END


#define DECLARE_MODEL_ITEM_LOCKER                                                            \
    public:                                                                                  \
        bool isLocked() const { return m_locker.isLocked(); }                                \
    	const QString &lockReason() const { return m_locker.lockReason(); }                  \
    	const QIcon &lockIcon() const { return m_locker.lockIcon(); }                        \
                                                                                             \
    private:                                                                                 \
        ::Tools::Models::ModelItemLocker m_locker                                            \


#define MODEL_ITEM_LOCKER_LOCK(reason, icon)                                                 \
	m_locker.lock(reason, icon)


#define MODEL_ITEM_LOCKER_UNLOCK                                                             \
	m_locker.unlock()

#endif /* DECLAREITEMLOCKER_H_ */
