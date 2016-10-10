#ifndef __CONFIG_H
#define __CONFIG_H
#include <string>
#include <list>
#include <vector>
#include <mutex>
#include <boost/property_tree/ptree.hpp>

class /*TC_API*/ ConfigMgr
{
	ConfigMgr() = default;
	ConfigMgr(ConfigMgr const&) = delete;
	ConfigMgr& operator=(ConfigMgr const&) = delete;
	~ConfigMgr() = default;

public:
	//初始化配置文件名
	bool LoadInitial(std::string const& file, std::vector<std::string> args,
		std::string& error);
	//单例方法
	static ConfigMgr* instance();

	bool Reload(std::string& error);

	std::string GetStringDefault(std::string const& name, const std::string& def) const;
	bool GetBoolDefault(std::string const& name, bool def) const;
	int GetIntDefault(std::string const& name, int def) const;
	float GetFloatDefault(std::string const& name, float def) const;

	std::string const& GetFilename();
	std::vector<std::string> const& GetArguments() const { return _args; }
	std::list<std::string> GetKeysByString(std::string const& name);

private:
	std::string _filename;
	std::vector<std::string> _args;
	boost::property_tree::ptree _config;
	std::mutex _configLock;

	template<class T>
	T GetValueDefault(std::string const& name, T def) const;
};

#define sConfigMgr ConfigMgr::instance()

#endif
