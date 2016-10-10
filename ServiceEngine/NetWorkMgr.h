#pragma once
#include "Public.h"
#include "AsyncListen.h"
#include "AsyncConnect.h"

class NetWorkMgr
{
public:
	typedef NetWorkMgr this_type;
	typedef boost::shared_ptr<tcp::socket> sock_ptr;

	SINGLE_INSTANCE(NetWorkMgr)

	NetWorkMgr();
	virtual ~NetWorkMgr();

	//加载一个新的异步监听
	bool NewAsyncListen(string name, uint16_t port);

	//加载一个新的异步连接
	bool NewAsyncConnect(string name, uint16_t port, string ip);

	//发送报文写入
	bool Writebuf(const bool isConnect
		, const string svrName
		, const uint32_t connectid
		, string& buf);

	//开始运行
	bool Start();
private:
	//网络任务队列
	boost::asio::io_service io_service_;
	//网络工作线程
	boost::shared_ptr<boost::thread> work_thread_;
	//异步监听列表
	map<string, boost::shared_ptr<AsyncListen>> listen_list_;
	//异步连接列表
	map<string, boost::shared_ptr<AsyncConnect>> connect_list_;
};

