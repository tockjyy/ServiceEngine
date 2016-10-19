#include "lua-lock.h"
#include "Public.h"
#include "Lock.h"


int NewSpinLock(lua_State *L)
{
	SpinLock* lock = new SpinLock;
	lock->lock.unlock();
	int plock = (int)lock;
	//cout << "lock:"<<plock << endl;
	lua_pushinteger(L, plock);
	return 1;
}

int NewRWLock(lua_State *L)
{
	WR_Lock* lock = new WR_Lock;
	int plock = (int)lock;
	lua_pushinteger(L, plock);
	return 1;
}

int DoSpinLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	//cout << "lock:" << plock << endl;
	SpinLock* lock = (SpinLock*)plock;
	lock->lock.lock();
	lua_pushboolean(L, 1);
	return 1;
}

int UnSpinLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	cout << "lock:" << plock << endl;
	SpinLock* lock = (SpinLock*)plock;
	lock->lock.unlock();
	lua_pushboolean(L, 1);
	return 1;
}

int ReadLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	WR_Lock* lock = (WR_Lock*)plock;
	lock->m_rwMutex.lock_shared();
	lua_pushboolean(L, 1);
	return 1;
}

int UnReadLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	WR_Lock* lock = (WR_Lock*)plock;
	lock->m_rwMutex.unlock_shared();
	lua_pushboolean(L, 1);
	return 1;
}

int WriteLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	WR_Lock* lock = (WR_Lock*)plock;
	lock->m_rwMutex.lock();
	lua_pushboolean(L, 1);
	return 1;
}

int UnWriteLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	WR_Lock* lock = (WR_Lock*)plock;
	lock->m_rwMutex.unlock();
	lua_pushboolean(L, 1);
	return 1;
}

static int DestorySpinLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	SpinLock* lock = (SpinLock*)plock;
	lock->lock.unlock();
	delete lock;
	lua_pushboolean(L, 1);
	return 1;
}

static int DestoryRWLock(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	int plock = lua_tointeger(L, 1);
	WR_Lock* lock = (WR_Lock*)plock;
	lock->m_rwMutex.unlock();
	delete lock;
	lua_pushboolean(L, 1);
	return 1;
}

int RegisterLock(lua_State* L) 
{
	luaL_Reg reg[] = {
		{ "NewSpinLock" , NewSpinLock },
		{ "DoSpinLock" , DoSpinLock },
		{ "UnSpinLock", UnSpinLock },
		{ "NewRWLock" , NewRWLock },
		{ "ReadLock" , ReadLock },
		{ "UnReadLock" , UnReadLock },
		{ "WriteLock", WriteLock },
		{ "UnWriteLock" , UnWriteLock },
		{ "DestorySpinLock" , DestorySpinLock },
		{ "DestoryRWLock" , DestoryRWLock },
		{ NULL,NULL },
	};

	luaL_checkversion(L);
	luaL_newlib(L, reg);

	return 1;
}