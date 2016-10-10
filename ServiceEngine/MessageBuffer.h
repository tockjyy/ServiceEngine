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
	//空间4096默认构造
	MessageBuffer() : _wpos(0), _rpos(0), _storage()
	{
		_storage.resize(4096);
	}
	//空间大小构造
	explicit MessageBuffer(std::size_t initialSize) : _wpos(0), _rpos(0), _storage()
	{
		_storage.resize(initialSize);
	}
	//复制构造
	MessageBuffer(MessageBuffer const& right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right._storage)
	{
	}
	//右值复制构造
	MessageBuffer(MessageBuffer&& right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right.Move()) { }

	void Reset()
	{
		_wpos = 0;
		_rpos = 0;
	}
	//改变空间大小
	void Resize(size_type bytes)
	{
		_storage.resize(bytes);
	}
	// 获得列表首元素地址
	uint8* GetBasePointer() { return _storage.data(); }
	// 获取未读数据首地址
	uint8* GetReadPointer() { return GetBasePointer() + _rpos; }
	// 获取已写数据末地址
	uint8* GetWritePointer() { return GetBasePointer() + _wpos; }
	// 已读空间增加
	void ReadCompleted(size_type bytes) { _rpos += bytes; }
	// 已写地址增加
	void WriteCompleted(size_type bytes) { _wpos += bytes; }
	// 过去尚未读取的空间数量
	size_type GetActiveSize() const { return _wpos - _rpos; }
	// 获取剩余空间
	size_type GetRemainingSpace() const { return _storage.size() - _wpos; }
	// 获取总空间大小
	size_type GetBufferSize() const { return _storage.size(); }

	// Discards inactive data
	// 将未读部分移动至空间起始地址，重置读写位
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

	// 如果空间不足，则扩展50%的空间
	void EnsureFreeSpace()
	{
		// resize buffer if it's already full
		if (GetRemainingSpace() == 0)
			_storage.resize(_storage.size() * 3 / 2);
	}
	//写到当前所写的位置上面
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
	//写到当前所写的位置上面
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
	//取右值
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
