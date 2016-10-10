#pragma once
#include "Public.h"
#include "Lock.h"
#include "Timer.h"

class AsyncConnect : public Timer , public SpinLock
{
public:
	AsyncConnect();
	~AsyncConnect();
	//定时重连
	virtual void TimerHandle() override;
	//初始化连接
	bool InitConnect(boost::asio::io_service* io_service_, string ip, uint16_t port, string name);
	void Read_Wait();
	void Read_Handle(boost::system::error_code error,
		size_t bytes_transferred);
	void Write_Wait();
	void Write_Handle(boost::system::error_code error,
		size_t bytes_transferred);
	//接收缓存数据是否完整
	bool LoseBufCheck(const uint16_t& size);
	//将报文写入发送缓存
	void WriteBuffer(const string& buf);
	//发送缓存数据是否完整
	bool CanSend();
	//接收报文处理方法
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

