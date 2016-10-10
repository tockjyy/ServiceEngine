#pragma once
extern "C"
{
#include <lua.h>
#include <lauxlib.h>  
#include <lualib.h>  
}
#include "Public.h"

#pragma comment(lib,"lua53.lib")

extern lua_State* L;

void RegisterFunc();

static int StartNewListen(lua_State *L);

static int StartNetWork(lua_State *L);

bool InitLua();

class LuaEngine
{
public:
	SINGLE_INSTANCE(LuaEngine)

	void StartThread(uint16_t thread_num_, lua_State *L);
	void Run(lua_State *L);
private:
	vector<lua_State*> state_list_;
	vector<boost::shared_ptr<boost::thread>> work_thread_;
};