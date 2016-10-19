#include "NetWorkMgr.h"



NetWorkMgr::NetWorkMgr()
{
}


NetWorkMgr::~NetWorkMgr()
{
}

//获取一个连接的远端IP地址
string NetWorkMgr::GetRemoveIp(string name, uint32_t conid)
{
	auto itr = listen_list_.find(name);
	if (itr == listen_list_.end())return "";
	return itr->second->GetRemoveIp(conid);
}

bool NetWorkMgr::NewAsyncListen(string name, uint16_t port, long ms)
{
	if (listen_list_.find(name) != listen_list_.end())return false;
	boost::shared_ptr<AsyncListen> _listen(new AsyncListen(&io_service_, port, name, ms));
	listen_list_.insert(pair<string, boost::shared_ptr<AsyncListen>>(name,_listen));
	return true;
}

bool NetWorkMgr::NewAsyncConnect(string name, uint16_t port, string ip)
{
	if (connect_list_.find(name) != connect_list_.end())return false;
	boost::shared_ptr<AsyncConnect> _connect(new AsyncConnect);
	if (!_connect->InitConnect(&io_service_, ip, port, name))return false;
	connect_list_.insert(pair<string, boost::shared_ptr<AsyncConnect>>(name, _connect));
	return true;
}


bool NetWorkMgr::Writebuf(const bool isConnect, const string svrName, const uint32_t connectid, string & buf)
{
	if (isConnect)
	{
		auto temp = connect_list_.find(svrName);
		if (temp == connect_list_.end())return false;
		temp->second->WriteBuffer(buf);
		return true;
	}
	else
	{
		auto temp = listen_list_.find(svrName);
		if (temp == listen_list_.end())return false;
		return temp->second->WriteBuffer(buf, connectid);
	}
}

bool NetWorkMgr::Start()
{
	boost::shared_ptr<boost::thread> _thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
	work_thread_ = _thread;
	return true;
}
