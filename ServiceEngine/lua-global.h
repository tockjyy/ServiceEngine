#pragma once
extern "C"
{
#include <lua.h>
#include <lauxlib.h>  
#include <lualib.h>  
}
#include "Public.h"
#include "Lock.h"
#include "TaskQueue.h"

#pragma comment(lib,"lua53.lib")

extern lua_State* gL;

//extern boost::detail::spinlock lock;

extern map<DWORD,lua_State*> state_list_;

int RegisterFunc(lua_State *L);

static int pcall_callback_err_fun(lua_State* L);

static int StartNewListen(lua_State *L);

static int GetRemoveIp(lua_State *L);

static int StartNewConnect(lua_State *L);

static int StartNetWork(lua_State *L);

static int SendMessage(lua_State *L);

static int SetAESKey(lua_State *L);

static int AESDeCrypt(lua_State *L);

static int AESEnCrypt(lua_State *L);

static int CreateSHA1(lua_State *L);

bool InitLua();

bool Start();

class LuaEngine
{
public:
	SINGLE_INSTANCE(LuaEngine)

	void StartThread(uint16_t thread_num_);
	void WorkRun();
	void Run();
	void MallocTask(shared_ptr<Task> task);
	boost::asio::io_service io_service_;
private:
	//stack<lua_State*> state_list_;
	shared_ptr<boost::thread> monitor_thread_;
	vector<boost::shared_ptr<boost::thread>> work_thread_;
};