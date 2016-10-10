import sys
import time
import threading
import ProtoHandle

class myThread (threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run(self):
        print "Starting a Thread"
        StartThread()
def StartThread():
	while 1:
		time.sleep(10)


class MonitorThread (threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run(self):
        print "Starting a Thread"
        StartMonitor()
def StartMonitor():
	while 1:
		task = ProtoHandle.CppEngine.GetRecvTask()
		obj = ProtoHandle.ProtoHandle(task)
		obj.Handle()

class ServiceMgr():
	threads = []
	threadLock = threading.RLock()

	def __init__(self,threadnum):
		ret = ProtoHandle.CppEngine.InitEngine()
		print(ret)
		ProtoHandle.CppEngine.StartNewListen("test",7777)
		ProtoHandle.CppEngine.StartNetWork()
		for num in range(1,threadnum):
			print str(num)+"Thread"
			thread = myThread()
			thread.start()
			ServiceMgr.threads.append(thread)

		thread = MonitorThread()
		thread.start()
		ServiceMgr.threads.append(thread)

		for t in ServiceMgr.threads:
			t.join()

		print("exiting main thread")