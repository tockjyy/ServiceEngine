#pragma once
extern "C"
{
#include <lua.h>
#include <lauxlib.h>  
#include <lualib.h>  
}
#pragma comment(lib,"lua53.lib")

int RegisterLock(lua_State* L);

static int NewSpinLock(lua_State *L);

static int NewRWLock(lua_State *L);

static int DestorySpinLock(lua_State *L);

static int DestoryRWLock(lua_State *L);

static int DoSpinLock(lua_State *L);

static int UnSpinLock(lua_State *L);

static int ReadLock(lua_State *L);

static int UnReadLock(lua_State *L);

static int WriteLock(lua_State *L);

static int UnWriteLock(lua_State *L);

