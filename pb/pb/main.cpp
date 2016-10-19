/* #include "pb.h"

int main(int argc, char* argv[])
{

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_pb(L);
	luaL_dofile(L, "main.lua");
	lua_pcall(L, 0, LUA_MULTRET, 0);
	lua_close(L);
	return 0;
}*/