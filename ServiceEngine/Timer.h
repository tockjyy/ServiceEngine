#ifndef __TIMER_H__
#define __TIMER_H__
#include "Public.h"

//定时器类
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
	//参数1 输入输出流指针 参数2 定时时间 = ms
	void StartTimerOver(boost::asio::io_service* io_service_, long msec_)
	{
		if (t_ != nullptr)
		{
			delete t_;
		}
		t_ = new boost::asio::deadline_timer(*io_service_, boost::posix_time::milliseconds(msec_));
		t_->async_wait(boost::bind(&Timer::func, this, msec_, true));
	};
	//参数1 输入输出流指针 参数2 定时时间 = ms
	void StartTimer(boost::asio::io_service* io_service_, long msec_)
	{
		if (t_ != nullptr)
		{
			return;
		}
		t_ = new boost::asio::deadline_timer(*io_service_, boost::posix_time::milliseconds(msec_));
		t_->async_wait(boost::bind(&Timer::func, this, msec_, true));
	};
	//参数1 输入输出流指针 参数2 定时时间 = ms
	void StartTimerOnce(boost::asio::io_service* io_service_, long msec_)
	{
		if (t_ != nullptr)
		{
			return;
		}
		t_ = new boost::asio::deadline_timer(*io_service_, boost::posix_time::milliseconds(msec_));
		t_->async_wait(boost::bind(&Timer::func, this, msec_, false));
	};
	void StopTimer()
	{
		if (t_ != nullptr)
		{
			delete t_;
			t_ = nullptr;
		}
	}
	void func(long msec_,bool again = true)
	{
		TimerHandle();
		if (!again)StopTimer();
		if (t_ == nullptr)
		{
			Destory();
			return;
		}
		t_->expires_at(t_->expires_at() + boost::posix_time::milliseconds(msec_));
		t_->async_wait(boost::bind(&Timer::func, this, msec_, again));
	};
	virtual void TimerHandle() {}//子类重写此方法再调用starttimer 即定时执行此方法
	virtual void Destory() {}
protected:
	boost::asio::deadline_timer* t_;
};

#endif
