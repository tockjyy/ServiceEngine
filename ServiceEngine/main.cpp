#include "lua-global.h"
#include "Public.h"

bool LoadConfig(const string& file)
{
	string error;
	if (!sConfigMgr->LoadInitial(file, vector<string>(), error))
	{
		cout << error << endl;
		return false;
	}
}

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		cout << "参数不正确,参数一为配置文件名称" << endl;
		getchar();
		return 0;
	}
	//初始化配置信息
	if (!LoadConfig(argv[1]))
	{
		cout << "ConfigFile Load Failed!" << endl;
		getchar();
		return 0;
	}

	// 初始化Lua
	if(!InitLua())
	{
		cout << "Lua Load Failed!" << endl;
		getchar();
		return 0;
	}
	Start();

	getchar();

	return 0;
}
