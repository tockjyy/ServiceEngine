#include "AsyncSession.h"
#include "TaskQueue.h"


AsyncSession::~AsyncSession()
{
}

AsyncSession::AsyncSession(boost::asio::io_service* _io_service, uint32_t id, this_pool_ptr pool, this_callback call, string name)
	: socket_(*_io_service)
	, rdata_()
	, wdata_()
	, connectid_(id)
	, is_write(false)
	, pool_(pool)
	, PoolCallBack_(call)
	, name_(name)
{
}
void AsyncSession::BeginHander()
{
	is_destory_ = false;
}
void AsyncSession::ConnectInit(boost::system::error_code error)
{
	rdata_.Reset();
	wdata_.Reset();
	BeginHander();
	Read_Wait(error);
}
void AsyncSession::Read_Wait(boost::system::error_code error)
{
	if (error)
	{
		ConnectDestroy();
	}
	else
	{
		rdata_.Normalize();
		rdata_.EnsureFreeSpace();
		socket_.async_read_some(boost::asio::buffer(rdata_.GetWritePointer(), rdata_.GetRemainingSpace()),
			boost::bind(&this_type::Read_Handle, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
}
void AsyncSession::Read_Handle(boost::system::error_code error,
	size_t bytes_transferred)
{
	if (error || bytes_transferred == 0)
	{
		ConnectDestroy();
	}
	else
	{
		rdata_.WriteCompleted(bytes_transferred);
		while (ProtocolHeadHandle()) {}
		Read_Wait(error);
	}
}
void AsyncSession::Write_Wait()
{
	is_write = true;
	boost::system::error_code error;
	boost::asio::async_write(socket_,
		boost::asio::buffer(wdata_.GetReadPointer(), wdata_.GetActiveSize()),
		boost::bind(&this_type::Write_Handle, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}
void AsyncSession::Write_Handle(boost::system::error_code error,
	size_t bytes_transferred)
{
	if (error || bytes_transferred == 0)
	{
		ConnectDestroy();
	}
	else
	{
		wdata_.ReadCompleted(bytes_transferred);
		wdata_.Normalize();
		wdata_.EnsureFreeSpace();
		is_write = false;
		if (CanSend())Write_Wait();
	}
}
//接收缓存数据是否完整
bool AsyncSession::LoseBufCheck(const uint16_t& size)
{
	if (size + 2 > (rdata_.GetActiveSize()))return false;
	return true;
}
//将报文写入发送缓存
void AsyncSession::WriteBuffer(MessageBuffer& buf)
{
	SPIN_LOCK();
	uint16_t size = buf.GetActiveSize();
	if (size == 0)return;
	wdata_.Write((void*)&size, 2);
	wdata_.Write(buf.GetReadPointer(), size);
	if ((!is_write) && (CanSend())) Write_Wait();
}
//将报文写入发送缓存
void AsyncSession::WriteBuffer(const string& buf)
{
	SPIN_LOCK();
	uint16_t size = buf.size();
	if (size == 0)return;
	wdata_.Write((void*)&size, 2);
	wdata_.Write(buf);
	if ((!is_write) && (CanSend())) Write_Wait();
}
//发送缓存数据是否完整
bool AsyncSession::CanSend()
{
	if (wdata_.GetActiveSize() < 2)return false;//报文头不全
	uint16_t data_size = 0;
	memcpy(&data_size, wdata_.GetReadPointer(), 2);
	if (wdata_.GetActiveSize() >= data_size + 2)return true;//报文内容不全
	return false;
}
//毁灭连接
void AsyncSession::ConnectDestroy()
{
	SPIN_LOCK();
	if (is_destory_)return;
	if (!socket_.is_open())return;
	socket_.shutdown(tcp::socket::shutdown_both);
	socket_.close();
	is_write = false;
	is_destory_ = !PoolCallBack_(pool_, CONNECT_CLOSE, connectid_);
	return;
}
//接收报文处理方法
bool AsyncSession::ProtocolHeadHandle()
{
	if (rdata_.GetActiveSize() < 2)return false;//报文头不全
	uint16_t data_size = 0;
	memcpy(&data_size, rdata_.GetReadPointer(), 2);
	if (!LoseBufCheck(data_size))return false;
	//**********run***********//
	GET_INSTANCE(TaskQueue)->CreateNewRecvTask(rdata_.GetReadPointer()+2,data_size, false, name_, connectid_);
	rdata_.ReadCompleted(data_size + 2);
	return true;
}