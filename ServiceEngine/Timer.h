#ifndef __TIMER_H__
#define __TIMER_H__
#include "Public.h"

//��ʱ����
class Timer
{
public:
	Timer() :t_(nullptr) {}
	~Timer()
	{
		if (t_ != nullptr)
		{
			delete t_;
		}
	}
	//����1 ���������ָ�� ����2 ��ʱʱ�� = ms
	void StartTimerOver(boost::asio::io_service* io_service_, long msec_)
	{
		if (t_ != nullptr)
		{
			delete t_;
		}
		t_ = new boost::asio::deadline_timer(*io_service_, boost::posix_time::milliseconds(msec_));
		t_->async_wait(boost::bind(&Timer::func, this, msec_));
	};
	//����1 ���������ָ�� ����2 ��ʱʱ�� = ms
	void StartTimer(boost::asio::io_service* io_service_, long msec_)
	{
		if (t_ != nullptr)
		{
			return;
		}
		t_ = new boost::asio::deadline_timer(*io_service_, boost::posix_time::milliseconds(msec_));
		t_->async_wait(boost::bind(&Timer::func, this, msec_));
	};
	void StopTimer()
	{
		if (t_ != nullptr)
		{
			delete t_;
			t_ = nullptr;
		}
	}
	void func(long msec_)
	{
		TimerHandle();
		if (t_ == nullptr)return;
		t_->expires_at(t_->expires_at() + boost::posix_time::milliseconds(msec_));
		t_->async_wait(boost::bind(&Timer::func, this, msec_));
	};
	virtual void TimerHandle() {}//������д�˷����ٵ���starttimer ����ʱִ�д˷���
protected:
	boost::asio::deadline_timer* t_;
};

#endif
