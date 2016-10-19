#pragma once
#include "Public.h"

#define SPIN_LOCK() SpinLock::spinLock guard(lock)
#define READ_LOCK() WR_Lock::readLock readlock(m_rwMutex)
#define WRITE_LOCK() WR_Lock::writeLock writelock(m_rwMutex)

//������--�ڷǳ�ʱ����ʱʹ�ã���д����Ƶ�ʸߵ������ʹ��
class SpinLock
{
public:
	typedef boost::lock_guard<boost::detail::spinlock>	spinLock;
public:
	boost::detail::spinlock lock;
};

//��д��--�ڶ�����Ƶ��Զ����дƵ��ʱʹ��
class WR_Lock
{
public:
	typedef boost::shared_mutex            WR_Mutex;
	typedef boost::unique_lock<WR_Mutex>   writeLock;
	typedef boost::shared_lock<WR_Mutex>   readLock;
public:
	WR_Mutex        m_rwMutex;
};