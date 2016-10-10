import test_pb2
import ServiceEngine
import sys

CppEngine = ServiceEngine.PythonEngine()

class ProtoHandle:    
    def __init__(self, task):    
        self.name = task.svrname
        self.iscont = task.iscont
        self.connectid = task.connectid
        self.buffer = task.buffer[2:]
        self.header = task.buffer[0:2]
    def Handle(self):
    	#address_book = addressbook_pb2.AddressBook()
    	print self.name
    	print self.connectid
    	print self.buffer
    	print self.header
    	#if self.header == test_pb2.TEST:
    	#	self.protobuf = test_pb2.student();
    	#	protobuf.ParseFromString(self.buffer)
    	#elif self.header == test_pb2.TEST: