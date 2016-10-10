#pragma once
#include "Public.h"

struct Task
{
public:
	bool isConnect_ = false;//�Ƿ��������׽���
	string svrName_ = "none";//��������
	uint32_t connectid_ = 0;//����ID
	string buffer = "noknow";
};

class TaskQueue
{
public:
	SINGLE_INSTANCE(TaskQueue)
	TaskQueue();
	virtual ~TaskQueue();

	//����ȡ����������������
	bool CreateNewRecvTask(const void* pbuf
		, const uint16_t& size
		, const bool isConnect
		, const string svrName
		, const uint32_t connectid);
	//����������
	inline bool HandleSendTask(string& buf
		, const bool isConnect
		, const string svrName
		, const uint32_t connectid);
	//����������������������
	bool CreateNewSendTask(shared_ptr<Task>& pTask);
	//�ӽ����б�����ȡһ������
	shared_ptr<Task> PopRecvTask();
private:
	typedef boost::mutex::scoped_lock AUTO_LOCK;

	//�ն���
	queue<shared_ptr<Task>> recv_task_list_;
	//���Ͷ���
	//queue<shared_ptr<Task>> send_task_list_;
	//�����߳�
	//boost::shared_ptr<boost::thread> work_thread_;
	//��������
	boost::condition_variable cv_;
	mutable boost::mutex read_mutex;
};

