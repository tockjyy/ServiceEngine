#pragma once
#include "Public.h"

struct Task
{
public:
	bool isConnect_ = false;//是否是连接套接字
	string svrName_ = "none";//服务名称
	uint32_t connectid_ = 0;//连接ID
	string buffer = "noknow";
};

class TaskQueue
{
public:
	SINGLE_INSTANCE(TaskQueue)
	TaskQueue();
	virtual ~TaskQueue();

	//往读取任务队列中添加任务
	bool CreateNewRecvTask(const void* pbuf
		, const uint16_t& size
		, const bool isConnect
		, const string svrName
		, const uint32_t connectid);
	//处理发送任务
	inline bool HandleSendTask(string& buf
		, const bool isConnect
		, const string svrName
		, const uint32_t connectid);
	//往发送任务队列中添加任务
	bool CreateNewSendTask(shared_ptr<Task>& pTask);
	//从接收列表中拿取一个任务
	shared_ptr<Task> PopRecvTask();
private:
	typedef boost::mutex::scoped_lock AUTO_LOCK;

	//收队列
	queue<shared_ptr<Task>> recv_task_list_;
	//发送队列
	//queue<shared_ptr<Task>> send_task_list_;
	//工作线程
	//boost::shared_ptr<boost::thread> work_thread_;
	//条件变量
	boost::condition_variable cv_;
	mutable boost::mutex read_mutex;
};

