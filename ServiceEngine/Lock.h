#pragma once
#include "Public.h"

#define SPIN_LOCK() SpinLock::spinLock guard(lock)
#define READ_LOCK() WR_Lock::readLock readlock(m_rwMutex)
#define WRITE_LOCK() WR_Lock::writeLock writelock(m_rwMutex)

//自旋锁--在非长时间锁时使用，在写数据频率高的情况下使用
class SpinLock
{
public:
	typedef boost::lock_guard<boost::detail::spinlock>	spinLock;
public:
	boost::detail::spinlock lock;
};

//读写锁--在读数据频率远高于写频率时使用
class WR_Lock
{
public:
	typedef boost::shared_mutex            WR_Mutex;
	typedef boost::unique_lock<WR_Mutex>   writeLock;
	typedef boost::shared_lock<WR_Mutex>   readLock;
public:
	WR_Mutex        m_rwMutex;
};