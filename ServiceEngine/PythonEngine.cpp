#include "PythonEngine.h"
#include "NetWorkMgr.h"
#include "TaskQueue.h"


// PythonEngine::PythonEngine():pModule_(NULL)
// {
// 	Py_Initialize();
// }
// PythonEngine::~PythonEngine()
// {
// 	Py_Finalize();
// 	delete pModule_;
// 	pModule_ = NULL;
// }
// // 
// bool PythonEngine::InitScript()
// {
// 	string mainscript = sConfigMgr->GetStringDefault("Main", "");
// 	if (mainscript == "")return false;
// 	if (pModule_)return false;
// 	pModule_ = new boost::python::handle<>(PyImport_ImportModule(mainscript.c_str()));
// 	//pModule_ = PyImport_ImportModule(mainscript.c_str());
// 	return true;
// }
/*调用
return boost::python::call_method<bool>(_module->get(), "initialize",
	config_file);
	PyObject *pClassPerson = PyObject_GetAttrString(pModule, "Person");
	//创建Person类的实例
	PyObject *pInstancePerson = PyInstance_New(pClassPerson, NULL, NULL);
	//调用方法
	PyObject_CallMethod(pInstancePerson, "greet", "s", "Hello Kitty");   //s表示传递的是字符串,值为"Hello Kitty"

*/

bool PythonEngine::StartNewListen(string name, uint16_t port)
{
	bool ret = GET_INSTANCE(NetWorkMgr)->NewAsyncListen(name, port);
	cout << "name:" << name << " port:" << port << " ret:" << ret << endl;
	return ret;
}

bool PythonEngine::StartNewConnect(string name, uint16_t port, string ip)
{
	bool ret = GET_INSTANCE(NetWorkMgr)->NewAsyncConnect(name, port, ip);
	cout << "name:" << name << " ip:" << ip << " port:" << port << " ret:" << ret << endl;
	return ret;
}

void PythonEngine::StartNetWork()
{
	GET_INSTANCE(NetWorkMgr)->Start();
}

bool PythonEngine::InitEngine()
{
// 	string error;
// 	if (!sConfigMgr->LoadInitial("Config.ini", vector<string>(), error))
// 	{
// 		cout << "ConfigFile Load Failed: " << error << endl;
// 		return false;
// 	}
	return true;
}

Task PythonEngine::GetRecvTask()
{
	shared_ptr<Task> temp = GET_INSTANCE(TaskQueue)->PopRecvTask();
	return std::move(*temp.get());
}

void PythonEngine::PushSendTask(Task & _task)
{
	shared_ptr<Task> temp(new Task);
	temp->isConnect_ = _task.isConnect_;
	temp->connectid_ = _task.connectid_;
	temp->svrName_ = _task.svrName_;
	temp->buffer = _task.buffer;
	GET_INSTANCE(TaskQueue)->CreateNewSendTask(temp);
}
