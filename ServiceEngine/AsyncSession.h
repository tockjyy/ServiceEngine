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
	//接收缓存数据是否完整
	bool LoseBufCheck(const uint16_t& size);
	//将报文写入发送缓存
	void WriteBuffer(MessageBuffer& buf);
	//将报文写入发送缓存
	void WriteBuffer(const string& buf);
	//发送缓存数据是否完整
	bool CanSend();
	//毁灭连接
	void ConnectDestroy();
	//接收报文处理方法
	bool ProtocolHeadHandle();
	//清理连接
	void ClearConnect(const uint32_t& sec,const time_t& time);
	//获得远端ip
	string GetRemoveIp();
	tcp::socket socket_;//套接字
	uint32_t connectid_;//线程池分配的连接ID
protected:
	this_pool_ptr pool_;
	this_callback PoolCallBack_;
	MessageBuffer rdata_;//接收缓存
	MessageBuffer wdata_;//发送缓存
	bool is_write, is_destory_;//共享变量 充当部分锁的作用
	string name_;
	time_t die_time_;
};
