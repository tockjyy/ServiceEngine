#include "lua-global.h"
#include "NetWorkMgr.h"
#include "TaskQueue.h"

lua_State* L;

void RegisterFunc()
{
	lua_register(L, "StartNewListen", StartNewListen);
	lua_register(L, "StartNetWork", StartNetWork);
}

int StartNewListen(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 2)
	{
		lua_pushboolean(L, 0);
	}
	string name = lua_tostring(L, 1);
	uint16_t port = lua_tonumber(L, 2);
	GET_INSTANCE(NetWorkMgr)->NewAsyncListen(name, port);
	lua_pushboolean(L, 1);
	return 1;
}

int StartNetWork(lua_State *L)
{
	//GET_INSTANCE(TaskQueue)->Start();
	GET_INSTANCE(NetWorkMgr)->Start();
	lua_pushboolean(L, 1);
	return 1;
}

bool InitLua()
{
	/* 初始化Lua */
	L = luaL_newstate();

	/* 载入Lua基本库 */
	luaL_openlibs(L);
	/* 注册函数 */
	RegisterFunc();
	/* 运行脚本 */
	string mainscript = sConfigMgr->GetStringDefault("Main", "");
	if (mainscript == "")return false;
	luaL_dofile(L, mainscript.c_str());
	GET_INSTANCE(LuaEngine)->StartThread(8, L);
	return true;
}

void LuaEngine::StartThread(uint16_t thread_num_,lua_State *L)
{
	for (int i(0); i < thread_num_; ++i)
	{
		lua_State *tL = lua_newthread(L);
		state_list_.push_back(tL);
		boost::shared_ptr<boost::thread> _thread
		(new boost::thread(boost::bind(&LuaEngine::Run, this, tL)));
		work_thread_.push_back(_thread);
	}
}

void LuaEngine::Run(lua_State *L)
{
	auto taskqueue_ = GET_INSTANCE(TaskQueue);
	while (1)
	{
		shared_ptr<Task> recv = taskqueue_->PopRecvTask();
		if (recv == shared_ptr<Task>())continue;
		lua_getglobal(L, "MessageHandle");
		lua_pushstring(L, recv->svrName_.c_str());
		lua_pushinteger(L, recv->connectid_);
		lua_pushboolean(L, recv->isConnect_);
// 		lua_pushlstring(L, (char*)recv->buffer.GetReadPointer(), recv->buffer.GetActiveSize());
// 		lua_pcall(L, 4, 4, 0);
// 		send->svrName_ = lua_tostring(L, 1);
// 		send->connectid_ = lua_tointeger(L, 2);
// 		send->isConnect_ = lua_toboolean(L, 3);
// 		string buf = lua_tostring(L, 4);
// 		send->buffer.Write(buf.c_str(), buf.size());
// 		taskqueue_->CreateNewSendTask(send);
	}
}
