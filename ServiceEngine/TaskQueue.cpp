#include "TaskQueue.h"
#include "NetWorkMgr.h"


TaskQueue::TaskQueue()
{
}


TaskQueue::~TaskQueue()
{
}

bool TaskQueue::CreateNewRecvTask(const void* pbuf
	, const uint16_t& size
	, const bool isConnect
	, const string svrName
	, const uint32_t connectid)
{
	shared_ptr<Task> temp(new Task);
	temp->isConnect_ = isConnect;
	temp->connectid_ = connectid;
	temp->svrName_ = svrName;
	temp->buffer.assign((char*)pbuf, size);
	{
		AUTO_LOCK lock(read_mutex);
		recv_task_list_.push(temp);
	}
	cv_.notify_one();
	return true;
}

bool TaskQueue::HandleSendTask(string& buf
	, const bool isConnect
	, const string svrName
	, const uint32_t connectid)
{
	return GET_INSTANCE(NetWorkMgr)->Writebuf(isConnect, svrName, connectid, buf);
}

bool TaskQueue::CreateNewSendTask(shared_ptr<Task>& pTask)
{
	HandleSendTask(pTask->buffer, pTask->isConnect_, pTask->svrName_, pTask->connectid_);
	return true;
}

shared_ptr<Task> TaskQueue::PopRecvTask()
{
	AUTO_LOCK lock(read_mutex);
	while (recv_task_list_.empty())
	{
		cv_.wait(lock);
	}
	auto temp = recv_task_list_.front();
	recv_task_list_.pop();
	return std::move(temp);
}

