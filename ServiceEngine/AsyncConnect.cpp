#include "AsyncConnect.h"
#include "TaskQueue.h"


AsyncConnect::AsyncConnect()
	: is_reconnect_(false)
	, is_write(false)
{
}


AsyncConnect::~AsyncConnect()
{
	if (socket_) delete socket_;
	if (ep_) delete ep_;
}

void AsyncConnect::TimerHandle()
{
	SPIN_LOCK();
	if (!is_reconnect_)
	{
		return;
	}
	if (socket_ && socket_->is_open())
	{
		socket_->shutdown(tcp::socket::shutdown_both);
		socket_->close();
	}
	if (socket_) delete socket_;
	if (ep_) delete ep_;
	ep_ = new boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip_), port_);
	socket_ = new boost::asio::ip::tcp::socket(*io_);
	boost::system::error_code error;
	socket_->connect(*ep_, error);
	if (error)
	{
		socket_->close();
		cout << "[Timer]:ReConnect is fails on " << ip_ << ":" << port_ << endl;
		return;
	}
	cout << "[Timer]:ReConnect is suceess on " << ip_ << ":" << port_ << endl;
	is_reconnect_ = false;
	wdata_.Reset();
	rdata_.Reset();
	Read_Wait();
	//if ((!is_write) && (CanSend())) Write_Wait();
	StopTimer();
}

bool AsyncConnect::InitConnect(boost::asio::io_service * io_service_, string ip, uint16_t port, string name)
{
	is_write = false;
	io_ = io_service_;
	name_ = name;
	port_ = port;
	ip_ = ip;
	if (!ep_)ep_ = new boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip_), port_);
	if (!socket_)socket_ = new boost::asio::ip::tcp::socket(*io_);
	boost::system::error_code error;
	socket_->connect(*ep_, error);
	if (error)
	{
		cout << "Connect is fails on " << ip_ << ":" << port_ << endl;
		return false;
	}
	is_reconnect_ = false;
	Read_Wait();
	//StartTimerOver(io_, 3000);
	return true;
}

void AsyncConnect::Read_Wait()
{
	rdata_.Normalize();
	rdata_.EnsureFreeSpace();
	socket_->async_read_some(boost::asio::buffer(rdata_.GetWritePointer(), rdata_.GetRemainingSpace()),
		boost::bind(&AsyncConnect::Read_Handle, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void AsyncConnect::Read_Handle(boost::system::error_code error, size_t bytes_transferred)
{
	if (error)
	{
		SPIN_LOCK();
		cout << "read" << endl;
		if (!is_reconnect_)
		{
			is_reconnect_ = true;
			StartTimerOver(io_, 3000);
		}
		is_write = false;
		return;
	}
	rdata_.WriteCompleted(bytes_transferred);
	while (ProtocolHeadHandle()) {}
	Read_Wait();
}

void AsyncConnect::Write_Wait()
{
	is_write = true;
	boost::asio::async_write(*socket_,
		boost::asio::buffer(wdata_.GetReadPointer(), wdata_.GetActiveSize()),
		boost::bind(&AsyncConnect::Write_Handle, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void AsyncConnect::Write_Handle(boost::system::error_code error, size_t bytes_transferred)
{
	if (error)
	{
		SPIN_LOCK();
		cout << "write" << endl;
		if (!is_reconnect_)
		{
			is_reconnect_ = true;
			StartTimerOver(io_, 3000);
		}
		is_write = false;
		return;
	}
	wdata_.ReadCompleted(bytes_transferred);
	wdata_.Normalize();
	wdata_.EnsureFreeSpace();
	is_write = false;
	if (CanSend())Write_Wait();
}

bool AsyncConnect::LoseBufCheck(const uint16_t & size)
{
	if (size + 2 > (rdata_.GetActiveSize()))return false;
	return true;
}

void AsyncConnect::WriteBuffer(const string & buf)
{
	SPIN_LOCK();
	if (is_reconnect_)return;
	uint16_t size = buf.size();
	if (size == 0)return;
	wdata_.Write((void*)&size, 2);
	wdata_.Write(buf);
	if ((!is_write) && (CanSend())) Write_Wait();
}

bool AsyncConnect::CanSend()
{
	if (wdata_.GetActiveSize() < 2)return false;//报文头不全
	uint16_t data_size = 0;
	memcpy(&data_size, wdata_.GetReadPointer(), 2);
	if (wdata_.GetActiveSize() >= data_size + 2)return true;//报文内容不全
	return false;
}

bool AsyncConnect::ProtocolHeadHandle()
{
	if (rdata_.GetActiveSize() < 2)return false;//报文头不全
	uint16_t data_size = 0;
	memcpy(&data_size, rdata_.GetReadPointer(), 2);
	if (!LoseBufCheck(data_size))return false;
	//**********run***********//
	GET_INSTANCE(TaskQueue)->CreateNewRecvTask(rdata_.GetReadPointer() + 2, data_size, true, name_, 0);
	rdata_.ReadCompleted(data_size + 2);
	return true;
}
