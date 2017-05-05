#include "lua-global.h"
#include "NetWorkMgr.h"
#include "TaskQueue.h"
#include "lua-lock.h"
#include "lua-timer.h"
#include "Cipher.h"
#include "pbc.h"

lua_State* gL;

//boost::detail::spinlock lock;

//map<DWORD, lua_State*> state_list_;

int pcall_callback_err_fun(lua_State* L)
{
	lua_Debug debug = {};
	uint32_t level = 0;
	while (lua_getstack(L, level, &debug)) {
		level++;
	}

	if (!level) {
		return 0;
	} 
	lua_getstack(L, level, &debug);// 0是pcall_callback_err_fun自己, 1是error函数, 2是真正出错的函数  
	lua_getinfo(L, "Sln", &debug);

	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << debug.short_src << ":line " << debug.currentline;
	if (debug.name != 0) {
		msg << "(" << debug.namewhat << " " << debug.name << ")";
	}

	msg << " [" << err << "]";
	lua_pushstring(L, msg.str().c_str());
	return 1;
}

int RegisterFunc(lua_State *L)
{
	luaL_Reg reg[] = {
		{ "StartNewListen" , StartNewListen },
		{ "StartNetWork" , StartNetWork },
		{ "GetRemoveIp", GetRemoveIp },
		{ "StartNewConnect" , StartNewConnect },
		{ "SendMessage" , SendMessage },
		{ "SetAESKey", SetAESKey },
		{ "AESDeCrypt" , AESDeCrypt },
		{ "AESEnCrypt" , AESEnCrypt },
		{ "CreateSHA1" , CreateSHA1 },
		{ NULL,NULL },
	};

	luaL_checkversion(L);
	luaL_newlib(L, reg);

	return 1;
}

int CreateSHA1(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n == 0)
	{
		lua_pushstring(L, "");
		return 1;
	}
	boost::uuids::detail::sha1 sha;
	for (int i(0); i < n; i++)
	{
		string str = lua_tostring(L, i+1);
		sha.process_bytes(str.c_str(),str.size());
	}
	unsigned int digest[5];
	sha.get_digest(digest);
	stringstream sstr;
	for (int i = 0; i < 5; ++i)
	{
		sstr << std::hex << digest[i];            // 以十六进制输出  
	}
	string str = sstr.str();
	lua_pushlstring(L, str.c_str(), str.size());
	return 1;
}

int SetAESKey(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	string key = lua_tostring(L, 1);
	auto ret = GET_INSTANCE(AES_128)->Set_Password(key);
	lua_pushboolean(L, ret);
	return 1;
}

int AESDeCrypt(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushstring(L, "");
		return 1;
	}
	string context = lua_tostring(L, 1);
	context = GET_INSTANCE(AES_128)->DeCrypt(context);
	lua_pushlstring(L, context.c_str(), context.size());
	return 1;
}

int AESEnCrypt(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 1)
	{
		lua_pushstring(L, "");
		return 1;
	}
	string context = lua_tostring(L, 1);
	context = GET_INSTANCE(AES_128)->EnCrypt(context);
	lua_pushlstring(L, context.c_str(), context.size());
	return 1;
}

int GetRemoveIp(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 2)
	{
		lua_pushstring(L, "");
		return 1;
	}
	string name = lua_tostring(L, 1);
	uint32_t conid = lua_tonumber(L, 2);
	auto ip = GET_INSTANCE(NetWorkMgr)->GetRemoveIp(name, conid);
	lua_pushstring(L, ip.c_str());
	return 1;
}

int StartNewListen(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 3)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	string name = lua_tostring(L, 1);
	uint16_t port = lua_tonumber(L, 2);
	uint32_t ms = lua_tonumber(L, 3);
	auto ret = GET_INSTANCE(NetWorkMgr)->NewAsyncListen(name, port,ms);
	lua_pushboolean(L, ret);
	return 1;
}

int StartNewConnect(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 3)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	string name = lua_tostring(L, 1);
	uint16_t port = lua_tonumber(L, 2);
	string ip = lua_tostring(L, 3);
	auto ret = GET_INSTANCE(NetWorkMgr)->NewAsyncConnect(name, port, ip);
	lua_pushboolean(L, ret);
	return 1;
}

int SendMessage(lua_State *L)
{
	uint16_t n = lua_gettop(L);
	if (n != 4)
	{
		lua_pushboolean(L, 0);
		return 1;
	}
	shared_ptr<Task> task(new Task);
	task->svrName_ = lua_tostring(L, 1);
	task->connectid_ = lua_tointeger(L, 2);
	task->isConnect_ = lua_toboolean(L, 3);
	task->buffer = lua_tostring(L, 4);
	auto ret = GET_INSTANCE(TaskQueue)->CreateNewSendTask(task);
	lua_pushboolean(L, ret);
	return 1;
}

int StartNetWork(lua_State *L)
{
	GET_INSTANCE(NetWorkMgr)->Start();
	uint16_t cpu = lua_tonumber(L, 1);
	GET_INSTANCE(LuaEngine)->StartThread(cpu);
	lua_pushboolean(L, 1);
	return 1;
}

bool Start() 
{
	try
	{
// 		lua_pushcfunction(gL, pcall_callback_err_fun);
// 		int pos_err = lua_gettop(gL);
		lua_getglobal(gL, "Start");
		//int s = lua_pcall(gL, 0, 0, pos_err);
		if (lua_pcall(gL, 0, LUA_MULTRET, 0) != 0) {
			std::cout << "[LUA ERROR]: " << endl << lua_tostring(gL, -1) << endl;
		}
// 		if (s != 0)
// 		{
// 			int t = lua_type(gL, -1);
// 			const char* err = lua_tostring(gL, -1);
// 			printf("Error: %s\n", err);
// 			lua_pop(gL, 1);
// 		}
	}
	catch (...)
	{
		cout << "Error calling lua function error";
	}
	return true;
}

bool InitLua()
{
	try
	{
		/* 初始化Lua */
		gL = luaL_newstate();

		/* 载入Lua基本库 */
		luaL_openlibs(gL);
		/* 注册网络函数 */
		luaL_requiref(gL, "cpp_socket", RegisterFunc, 9);
		/* 注册锁函数 */
		luaL_requiref(gL, "cpp_lock", RegisterLock, 10);
		/* 注册定时器函数 */
		luaL_requiref(gL, "cpp_timer", RegisterTimer, 2);
		/* 注册protobuf函数 */
		luaL_requiref(gL, "protobuf\.c", luaopen_protobuf_c, 29);
		/* 运行脚本 */
		string mainscript = sConfigMgr->GetStringDefault("Main", "");
		if (mainscript == "")return false;
		if (luaL_loadfile(gL, mainscript.c_str()) != 0) {
			std::cout << "[LUA ERROR]: "<< endl << lua_tostring(gL, -1) << endl;
		}
		if (lua_pcall(gL, 0, LUA_MULTRET, 0) != 0) {
			std::cout << "[LUA ERROR]: "<< endl << lua_tostring(gL, -1) << endl;
		}
	}
	catch (...)
	{
		cout << "Error calling lua function error";
	}
	return true;
}

void LuaEngine::StartThread(uint16_t thread_num_)
{
	//shared_ptr<boost::thread> thread
	//(new boost::thread(boost::bind(&LuaEngine::Run, this)));
	//monitor_thread_ = thread;
	StartTimer(&io_service_, 1000);
	for (int i(0); i < thread_num_; ++i)
	{
		lua_State *L;
		L = lua_newthread(gL);
		state_list.push(L);
		//lua_State *tL = lua_newthread(gL);
		//state_list_.push(tL);
		boost::thread _thread(boost::bind(&LuaEngine::WorkRun, this));
		work_thread_.push_back(_thread);
	}
}

void LuaEngine::WorkRun()
{
	auto id = GetCurrentThreadId();
	cout << "thread :" << id << " is open" << endl;
	io_service_.run();
	cout << "thread :" << id << " is close" << endl;
}

void LuaEngine::Run()
{
	auto taskqueue_ = GET_INSTANCE(TaskQueue);
	while (1)
	{
// 		lua_getglobal(L, "Test");
// 		lua_pcall(L, 0, 0, 0);
// 		Sleep(1000);
// 		int64_t count(0);
// 		auto time1 = clock();
// 		auto it = xx.find(10000);
// 		auto time2 = clock();
		//cout << "time: " << clock() - time1 << endl;
 		shared_ptr<Task> recv = taskqueue_->PopRecvTask();
		io_service_.post(boost::bind(&LuaEngine::MallocTask, this, move(recv)));
	}
}

void LuaEngine::MallocTask(shared_ptr<Task> task)
{
	try
	{
//		lock.lock();
// 		auto L = state_list_.top();
// 		state_list_.pop();
// 		lock.unlock();
		lock.lock();
		lua_State *L;
		L = state_list.front();
		state_list.pop();
		auto id = GetCurrentThreadId();
		cout << "count: " << work_thread_.size() <<" "<< id << endl;
		lock.unlock();
// 		auto itr = state_list_.find(id);
// 		if (itr == state_list_.end())
// 		{
// 			L = lua_newthread(gL);
// 			state_list_.insert(pair<DWORD, lua_State*>(id, L));
// 		}
// 		else
// 		{
// 			L = itr->second;
// 		}
		lua_getglobal(L, "MessageHandle");
		lua_pushstring(L, task->svrName_.c_str());
		lua_pushinteger(L, task->connectid_);
		lua_pushboolean(L, task->isConnect_);
		lua_pushlstring(L, task->buffer.c_str(), task->buffer.size());
		if (lua_pcall(L, 4, LUA_MULTRET, 0) != 0) {
			std::cout << "[LUA ERROR]: " << endl << lua_tostring(L, -1) << endl;
		}
		//lua_pcall(L, 4, 0, 0);
		lock.lock();
		state_list.push(L);
		lock.unlock();
	}
	catch (...)
	{
		cout << "Error calling lua function error";
	}
}