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
		cout << "��������ȷ,����һΪ�����ļ�����" << endl;
		getchar();
		return 0;
	}
	//��ʼ��������Ϣ
	if (!LoadConfig(argv[1]))
	{
		cout << "ConfigFile Load Failed!" << endl;
		getchar();
		return 0;
	}

	// ��ʼ��Lua
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
