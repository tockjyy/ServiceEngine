#include "lua-timer.h"
#include "lua-global.h"

unordered_map<string, unordered_map<uint32_t,shared_ptr<Lua_Timer>>> time_list_;
boost::detail::spinlock timelock;

Lua_Timer::Lua_Timer(string func, uint32_t index) :Timer()
, func_(func)
, index_(index) {}
bool Lua_Timer::NewLuaTimer(bool once, long msec_, string func, uint32_t index)
{
	shared_ptr<Lua_Timer> p(new Lua_Timer(func, index));
	timelock.lock();
	auto ret = time_list_.insert(
		pair <string, unordered_map<uint32_t, shared_ptr<Lua_Timer>>>
		(func, unordered_map<uint32_t, shared_ptr<Lua_Timer>>())
	);
	auto& ret2 = ret.first->second;
	if (ret2.insert(pair<uint32_t, shared_ptr<Lua_Timer>>(index, p)).second == false)
	{
		timelock.unlock();
		return false;
	}
	timelock.unlock();
	if (once)
	{
		p->StartTimerOnce(&GET_INSTANCE(LuaEngine)->io_service_, msec_);
	}
	else
	{
		p->StartTimer(&GET_INSTANCE(LuaEngine)->io_service_, msec_);
	}
	return true;
}
void Lua_Timer::TimerHandle()
{
// 	lock.lock();
// 	auto L = state_list_.top();
// 	state_list_.pop();
// 	lock.unlock();
// 	lua_State *L;
// 	auto id = GetCurrentThreadId();
// 	auto itr = state_list_.find(id);
// 	if (itr == state_list_.end())
// 	{
// 		L = lua_newthread(gL);
// 		state_list_.insert(pair<DWORD, lua_State*>(id, L));
// 	}
// 	else
// 	{
// 		L = itr->second;
// 	}
	auto instance_ = GET_INSTANCE(LuaEngine);
	instance_->lock.lock();
	lua_State *L;
	L = instance_->state_list.front();
	instance_->state_list.pop();
	instance_->lock.unlock();
	lua_getglobal(L, func_.c_str());
	lua_pushinteger(L, index_);
	if (lua_pcall(L, 1, LUA_MULTRET, 0) != 0) {
		std::cout << "[LUA ERROR]: " << endl << lua_tostring(L, -1) << endl;
	}
	else
	{
		bool ret = lua_toboolean(L, -1);
		if (!ret)
		{
			cout << "timer will close!" << endl;
			StopTimer();
		}
	}
	//lua_pcall(L, 1, 1, 0);
	instance_->lock.lock();
	instance_->state_list.push(L);
	instance_->lock.unlock();
}
void Lua_Timer::Destory()
{
	boost::lock_guard<boost::detail::spinlock> grand(timelock);
	auto it = time_list_.find(func_);
	if (it == time_list_.end())return;
	auto it2 = it->second.find(index_);
	if (it2 == it->second.end())return;
	it->second.erase(it2);
	if (it->second.empty())
		time_list_.erase(it);
}

int RegisterTimer(lua_State* L)
{
	luaL_Reg reg[] = {
		{ "NewTimer" , NewTimer },
		{ "NewOnceTimer" , NewOnceTimer },
		{ NULL,NULL },
	};

	luaL_checkversion(L);
	luaL_newlib(L, reg);

	return 1;
}

static int NewTimer(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 3)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	string func = lua_tostring(L, 1);
	uint32_t index = lua_tonumber(L, 2);
	long ms = lua_tonumber(L, 3);
	auto ret = Lua_Timer::NewLuaTimer(false, ms, func, index);
	lua_pushboolean(L, ret);
	return 1;
}

static int NewOnceTimer(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 3)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	string func = lua_tostring(L, 1);
	uint32_t index = lua_tonumber(L, 2);
	long ms = lua_tonumber(L, 3);
	auto ret = Lua_Timer::NewLuaTimer(true, ms, func, index);
	lua_pushboolean(L, ret);
	return 1;
}