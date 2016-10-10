#pragma once
#include "Public.h"
#include "Lock.h"
#include "Log.h"

template <typename TConnect>
class ConnectPool : public SpinLock
{
public:
	//�ͷſ�������//��ʱ�޳��ͷ�,������Ӧ��������
	void FreeSpace() {}
	//
	ConnectPool(boost::asio::io_service * io,string name)
		: pio_(io)
		, autovalue_(0)
		, count_(0)
		, name_(name)
	{
	}
	TConnect* MallocSpace()
	{
		SPIN_LOCK();
		//������ж��д�СС�����ӳ�����������1/3 ��Ҫ�·���1/3�Ŀ�������
		if (((IdleList_.size() * 3) <= count_))
		{
			uint32_t num = (count_ / 3) + 1;
			for (uint32_t i(0); i < num; i++)
			{
				TConnect* tConnect = new TConnect(pio_, ++autovalue_, this, &ConnectPool::ContCallback,name_);
				IdleList_.insert(pair<uint32_t, TConnect*>(tConnect->connectid_, tConnect));
			}
			stringstream message;
			message << "���ӳ���չ,ԭ��" << count_ << "������,������չ" << num << "��";
			count_ += num;
			GET_INSTANCE(Log)->OUT_LOG(message.str(), LOG_NOTE);
		}
		map<uint32_t, TConnect*>::iterator it = IdleList_.begin();
		TConnect* session = it->second;
		IdleList_.erase(it);
		return session;
	}

	void AddBusy(TConnect * session)
	{
		SPIN_LOCK();
		BusyList_.insert(pair<uint32_t, TConnect*>(session->connectid_, session));
	}

	bool WriteBuffer(string& buf, const uint32_t connectid)
	{
		SPIN_LOCK();
		auto temp = BusyList_.find(connectid);
		if (temp == BusyList_.end())return false;
		temp->second->WriteBuffer(buf);
		return true;
	}

	bool ContCallback(uint32_t cmd, uint32_t id)
	{
		SPIN_LOCK();
		if (cmd == CONNECT_CLOSE)//���ӶϿ���Ϣ
		{
			auto it = BusyList_.find(id);
			if (it != BusyList_.end())
			{
				auto result = IdleList_.insert(pair<uint32_t, TConnect*>(it->first, it->second));
				if (!result.second) return false;
				BusyList_.erase(it);
				stringstream message;
				message << "���� " << it->first << " ��������";
				GET_INSTANCE(Log)->OUT_LOG(message.str(), LOG_NOTE);
			}
			else return false;
		}
		return true;
	}
	virtual ~ConnectPool(){}
protected:
	uint32_t count_; //��ǰ���ӳش�С
	uint32_t autovalue_; //���Ӻ�--����������

	map<uint32_t, TConnect*> BusyList_; //��æ����
	map<uint32_t, TConnect*> IdleList_; //���ж���

	boost::asio::io_service* pio_; //�������ָ��
	string name_;
};

