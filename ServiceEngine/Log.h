#pragma once
#include "Public.h"
#include "Lock.h"
#include <direct.h>

class Log : public SpinLock
{
public:
	SINGLE_INSTANCE(Log)
	bool LoadInitial()
	{
		SPIN_LOCK();
		uint16_t logcnt_ = sConfigMgr->GetIntDefault("LogFileNum", 0);
		if (logcnt_ <= 0)return false;
		_mkdir("Log");
		for (uint16_t i(1); i <= logcnt_; i++)
		{
			stringstream str;
			str << "LogFile" << i;
			string logfile = sConfigMgr->GetStringDefault(str.str(), "");
			if (logfile == "")continue;
			logfilename_.insert(pair<uint16_t, string>(i, logfile));
		}
		return true;
	}

	void OUT_LOG(const string& str, const uint16_t& status, const uint16_t& type = 0)
	{
		SPIN_LOCK();
		string name;
		if (type != 0)
		{
			auto itr = logfilename_.find(type);
			if (itr == logfilename_.end())return;
			name = itr->second;
		}
		else
		{
			name = "System";
		}
		string prompt("[NOKNOW]");
		switch (status)
		{
		case LOG_NOTE:prompt = "[NOTE]   "; break;
		case LOG_WARNING:prompt = "[WARNING]"; break;
		case LOG_ERROR:prompt = "[ERROR]  "; break;
		default:break;
		}
		{
			auto nowtime = time(NULL);
			auto nowtm = localtime(&nowtime);
			stringstream timesstr;
			timesstr << nowtm->tm_year + 1900 << "-" << nowtm->tm_mon + 1 << "-"
				<< nowtm->tm_mday << "-" << nowtm->tm_hour;
			string timestr = timesstr.str();
			stringstream filename;
			filename << ".\\Log\\" << name << "-" << timestr << ".log";
			std::ofstream file(filename.str(), ios::app);
			file << prompt << " [" << timestr << "-" << nowtm->tm_min << "-" << nowtm->tm_sec << "]:\t" << str << endl;
			file.close();
		}
	}

	virtual ~Log()
	{
	}
protected:
	map<uint16_t, string> logfilename_;
};


