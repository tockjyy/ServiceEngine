#pragma once
#include "Public.h"
#include "AsyncSession.h"

class AsyncListen : public ConnectPool<AsyncSession>
{
public:
	typedef AsyncListen this_type;
	typedef boost::shared_ptr<tcp::socket> sock_ptr;

	virtual ~AsyncListen() {}
	AsyncListen(boost::asio::io_service * io, uint16_t port, string name, long ms)
		: acceptor_(*io, tcp::endpoint(tcp::v4(), port))
		, pio_(io)
		, ConnectPool(io,name,ms)
	{
		Start_Accept();
	}
	void Start_Accept()
	{
		MallocSpace();
		AsyncSession* session = curCont;
		acceptor_.async_accept(session->socket_,
			bind(&this_type::Handle_Accept, this,
				boost::asio::placeholders::error, session));
	}

	void Handle_Accept(boost::system::error_code error, AsyncSession * session)
	{
		if (!error)
		{
			AddBusy(session);//�������ӵ�ʵ�����뷱æ����
			session->ConnectInit(error);//�������ӿ�ʼ�첽��������
		}
		Start_Accept();//���ӷ���ɹ����ظ�����
	}
private:
	boost::asio::io_service* pio_;
	tcp::acceptor acceptor_;
};

