#pragma once
extern "C"
{
#include <lua.h>
#include <lauxlib.h>  
#include <lualib.h>  
}
#include "Public.h"
#include "Timer.h"
#pragma comment(lib,"lua53.lib")

class Lua_Timer : public Timer
{
public:
	Lua_Timer(string func, uint32_t index);
	static bool NewLuaTimer(bool once, long msec_, string func, uint32_t index);
	virtual void TimerHandle() override;
	virtual void Destory() override;
private:
	string func_;
	uint32_t index_;
};

int RegisterTimer(lua_State* L);

static int NewTimer(lua_State *L);

static int NewOnceTimer(lua_State *L);