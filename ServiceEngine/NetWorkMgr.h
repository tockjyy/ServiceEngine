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

	//����һ���µ��첽����
	bool NewAsyncListen(string name, uint16_t port);

	//����һ���µ��첽����
	bool NewAsyncConnect(string name, uint16_t port, string ip);

	//���ͱ���д��
	bool Writebuf(const bool isConnect
		, const string svrName
		, const uint32_t connectid
		, string& buf);

	//��ʼ����
	bool Start();
private:
	//�����������
	boost::asio::io_service io_service_;
	//���繤���߳�
	boost::shared_ptr<boost::thread> work_thread_;
	//�첽�����б�
	map<string, boost::shared_ptr<AsyncListen>> listen_list_;
	//�첽�����б�
	map<string, boost::shared_ptr<AsyncConnect>> connect_list_;
};

