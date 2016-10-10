#pragma once
#include "Public.h"
#include "Lock.h"
#include "Timer.h"

class AsyncConnect : public Timer , public SpinLock
{
public:
	AsyncConnect();
	~AsyncConnect();
	//��ʱ����
	virtual void TimerHandle() override;
	//��ʼ������
	bool InitConnect(boost::asio::io_service* io_service_, string ip, uint16_t port, string name);
	void Read_Wait();
	void Read_Handle(boost::system::error_code error,
		size_t bytes_transferred);
	void Write_Wait();
	void Write_Handle(boost::system::error_code error,
		size_t bytes_transferred);
	//���ջ��������Ƿ�����
	bool LoseBufCheck(const uint16_t& size);
	//������д�뷢�ͻ���
	void WriteBuffer(const string& buf);
	//���ͻ��������Ƿ�����
	bool CanSend();
	//���ձ��Ĵ�����
	bool ProtocolHeadHandle();
protected:
	MessageBuffer rdata_;
	MessageBuffer wdata_;
	bool is_write,is_reconnect_;
	boost::asio::io_service* io_;
	boost::asio::ip::tcp::socket* socket_;
	boost::asio::ip::tcp::endpoint* ep_;
	uint16_t port_;
	string ip_;
	string name_;
};

