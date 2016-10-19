#pragma once
#include "Public.h"
#include "Lock.h"
#include "ConnectPool.h"

class AsyncSession : public WR_Lock
{
public:
	typedef AsyncSession this_type;
	typedef boost::shared_ptr<tcp::socket> sock_ptr;
	typedef ConnectPool<AsyncSession>* this_pool_ptr;
	typedef boost::function<bool(this_pool_ptr, uint32_t, uint32_t)> this_callback;

	virtual ~AsyncSession();
	AsyncSession(boost::asio::io_service* _io_service, uint32_t id, this_pool_ptr pool, this_callback call,string name);
	void BeginHander();
	void ConnectInit(boost::system::error_code error);
	void Read_Wait(boost::system::error_code error);
	void Read_Handle(boost::system::error_code error,
		size_t bytes_transferred);
	void Write_Wait();
	void Write_Handle(boost::system::error_code error,
		size_t bytes_transferred);
	//���ջ��������Ƿ�����
	bool LoseBufCheck(const uint16_t& size);
	//������д�뷢�ͻ���
	void WriteBuffer(MessageBuffer& buf);
	//������д�뷢�ͻ���
	void WriteBuffer(const string& buf);
	//���ͻ��������Ƿ�����
	bool CanSend();
	//��������
	void ConnectDestroy();
	//���ձ��Ĵ�����
	bool ProtocolHeadHandle();
	//��������
	void ClearConnect(const uint32_t& sec,const time_t& time);
	//���Զ��ip
	string GetRemoveIp();
	tcp::socket socket_;//�׽���
	uint32_t connectid_;//�̳߳ط��������ID
protected:
	this_pool_ptr pool_;
	this_callback PoolCallBack_;
	MessageBuffer rdata_;//���ջ���
	MessageBuffer wdata_;//���ͻ���
	bool is_write, is_destory_;//������� �䵱������������
	string name_;
	time_t die_time_;
};
