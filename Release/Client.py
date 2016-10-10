import sys
import time
sys.path.append("C:\Python27")
sys.path.append(".\Python\Client")
#import ProtoHandle
import ServiceEngine
print("engine is open")
CppEngine = ServiceEngine.PythonEngine()

def __main__():
	ret = CppEngine.InitEngine()
	print(ret)
	CppEngine.StartNewConnect("test",7777,"192.168.2.21")
	CppEngine.StartNetWork()
	while 1:
		obj = ServiceEngine.PyTask()
		obj.buffer = "laosiji"
		obj.iscont = 1
		obj.connectid = 0
		obj.svrname = "test"
		time.sleep(1)
		CppEngine.PushSendTask(obj)

__main__()