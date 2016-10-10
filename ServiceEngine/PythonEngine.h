#pragma once
#include "Public.h"
#include "TaskQueue.h"
//#include <boost/python/module.hpp>
//#include <Python.h>
//#pragma comment(lib,"python27.lib")
//#pragma comment(lib,"ws2_32.lib")

// void StartNewListen(string name, uint16_t port);
// 
// void StartNetWork();
// 
// bool InitEngine();
// 
// BOOST_PYTHON_MODULE(ServiceEngine)
// {
// 	boost::python::def("StartNewListen", StartNewListen,
// 		boost::python::args("name"),
// 		boost::python::args("port"));
// 	boost::python::def("StartNetWork", StartNetWork);
// 	boost::python::def("InitEngine", InitEngine);
// }


class PythonEngine
{
public:
	SINGLE_INSTANCE(PythonEngine)

	bool StartNewListen(string name, uint16_t port);
	bool StartNewConnect(string name, uint16_t port, string ip);
	void StartNetWork();
	bool InitEngine();
	Task GetRecvTask();
	void PushSendTask(Task& _task);
private:
	vector<boost::shared_ptr<boost::thread>> thread_queue_;
	boost::python::handle<PyObject>* pModule_;
};

BOOST_PYTHON_MODULE(ServiceEngine)
{
	using namespace boost::python;
	class_<PythonEngine>("PythonEngine")
		.def("StartNewListen", &PythonEngine::StartNewListen,
			boost::python::args("name"),
			boost::python::args("port"))
		.def("StartNewConnect", &PythonEngine::StartNewConnect,
			boost::python::args("name"),
			boost::python::args("port"),
			boost::python::args("ip"))
		.def("StartNetWork", &PythonEngine::StartNetWork)
		.def("InitEngine", &PythonEngine::InitEngine)
		.def("GetRecvTask", &PythonEngine::GetRecvTask)
		.def("PushSendTask", &PythonEngine::PushSendTask,
			boost::python::args("task"));
	class_<Task>("PyTask")
		.def_readwrite("buffer", &Task::buffer)
		.def_readwrite("iscont", &Task::isConnect_)
		.def_readwrite("connectid", &Task::connectid_)
		.def_readwrite("svrname", &Task::svrName_);
}

