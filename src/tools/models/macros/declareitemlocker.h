#ifndef DECLAREITEMLOCKER_H_
#define DECLAREITEMLOCKER_H_

#include <QtCore/QString>
#include <QtCore/QModelIndex>
#include "../models_ns.h"


MODELS_NS_BEGIN

struct ModelItemLocker
{
	ModelItemLocker() :
		m_locked(false)
	{}

	bool isLocked() const { return m_locked; }
	const QString &lockReason() const { return m_reason; }

	void lock(const QString &reason) { m_reason = reason; m_locked = true; }
	void unlock() { m_locked = false; m_reason.clear(); }

private:
	bool m_locked;
	QString m_reason;
};

MODELS_NS_END


#define DECLARE_MODEL_ITEM_LOCKER                                           \
    public:                                                                 \
        bool isLocked() const { return m_locker.isLocked(); }               \
    	const QString &lockReason() const { return m_locker.lockReason(); } \
                                                                            \
		void lock(const QString &reason) { m_locker.lock(reason); }         \
        void unlock() { m_locker.unlock(); }                                \
		                                                                    \
    private:                                                                \
        ::Tools::Models::ModelItemLocker m_locker                           \

#endif /* DECLAREITEMLOCKER_H_ */
