#ifndef __MESSAGEBUFFER_H_
#define __MESSAGEBUFFER_H_

typedef unsigned char uint8;

#include <vector>
#include <cstring>
#include <string>

using std::string;

class MessageBuffer
{
	typedef std::vector<uint8>::size_type size_type;

public:
	//�ռ�4096Ĭ�Ϲ���
	MessageBuffer() : _wpos(0), _rpos(0), _storage()
	{
		_storage.resize(4096);
	}
	//�ռ��С����
	explicit MessageBuffer(std::size_t initialSize) : _wpos(0), _rpos(0), _storage()
	{
		_storage.resize(initialSize);
	}
	//���ƹ���
	MessageBuffer(MessageBuffer const& right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right._storage)
	{
	}
	//��ֵ���ƹ���
	MessageBuffer(MessageBuffer&& right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right.Move()) { }

	void Reset()
	{
		_wpos = 0;
		_rpos = 0;
	}
	//�ı�ռ��С
	void Resize(size_type bytes)
	{
		_storage.resize(bytes);
	}
	// ����б���Ԫ�ص�ַ
	uint8* GetBasePointer() { return _storage.data(); }
	// ��ȡδ�������׵�ַ
	uint8* GetReadPointer() { return GetBasePointer() + _rpos; }
	// ��ȡ��д����ĩ��ַ
	uint8* GetWritePointer() { return GetBasePointer() + _wpos; }
	// �Ѷ��ռ�����
	void ReadCompleted(size_type bytes) { _rpos += bytes; }
	// ��д��ַ����
	void WriteCompleted(size_type bytes) { _wpos += bytes; }
	// ��ȥ��δ��ȡ�Ŀռ�����
	size_type GetActiveSize() const { return _wpos - _rpos; }
	// ��ȡʣ��ռ�
	size_type GetRemainingSpace() const { return _storage.size() - _wpos; }
	// ��ȡ�ܿռ��С
	size_type GetBufferSize() const { return _storage.size(); }

	// Discards inactive data
	// ��δ�������ƶ����ռ���ʼ��ַ�����ö�дλ
	void Normalize()
	{
		if (_rpos)
		{
			if (_rpos != _wpos)
				memmove(GetBasePointer(), GetReadPointer(), GetActiveSize());
			_wpos -= _rpos;
			_rpos = 0;
		}
	}

	// ����ռ䲻�㣬����չ50%�Ŀռ�
	void EnsureFreeSpace()
	{
		// resize buffer if it's already full
		if (GetRemainingSpace() == 0)
			_storage.resize(_storage.size() * 3 / 2);
	}
	//д����ǰ��д��λ������
	void Write(void const* data, std::size_t size)
	{
		if (size)
		{
			while (size >= GetRemainingSpace())
			{
				_storage.resize(_storage.size() * 3 / 2);
			}
			memcpy(GetWritePointer(), data, size);
			WriteCompleted(size);
		}
	}
	//д����ǰ��д��λ������
	void Write(const string& str)
	{
		std::size_t size = str.size();
		if (size)
		{
			while (size >= GetRemainingSpace())
			{
				_storage.resize(_storage.size() * 3 / 2);
			}
			memcpy(GetWritePointer(), str.c_str(), size);
			WriteCompleted(size);
		}
	}
	//ȡ��ֵ
	std::vector<uint8>&& Move()
	{
		_wpos = 0;
		_rpos = 0;
		return std::move(_storage);
	}

	MessageBuffer& operator=(MessageBuffer const& right)
	{
		if (this != &right)
		{
			_wpos = right._wpos;
			_rpos = right._rpos;
			_storage = right._storage;
		}

		return *this;
	}

	MessageBuffer& operator=(MessageBuffer&& right)
	{
		if (this != &right)
		{
			_wpos = right._wpos;
			_rpos = right._rpos;
			_storage = right.Move();
		}

		return *this;
	}
private:
	size_type _wpos;
	size_type _rpos;
	std::vector<uint8> _storage;
};

#endif /* __MESSAGEBUFFER_H_ */
