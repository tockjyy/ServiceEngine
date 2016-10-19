package.path = package.path..';.\\lua\\gate\\?.lua'..';.\\lua\\proto\\?.lua'
local config = require("ServiceConfig")
local protohandle = require("ProtoHandle")
local package = require("Package")
local proto = require("Proto")
local cpp = require("cpp_socket")
local timer = require("cpp_timer")

function Start()

	if cpp.StartNewConnect("MgrGate_Gate",ServicePort["MgrGate_Gate"],MgeGateAddress) == false then
		print("MgrGate-Gate Start Error!")
	else
		print("MgrGate-Gate Start Sueecss!")
	end

	if cpp.StartNewListen("Client_Gate",ServicePort["Client_Gate"],30) == false then
		print("Client-Gate Start Error!")
	else
		print("Client-Gate Start Sueecss!")
	end
	timer.NewTimer("time",0,10000)
	if cpp.StartNetWork(8) == false then
		print("StartNetWork Error!")
	else
		print("StartNetWork Sueecss!")
	end

	local buf = package.PackageProduct(gate_mgrgate_proto["REGGATE"],"")
	print(cpp.SendMessage("MgrGate_Gate",0,true,buf),"buf:",buf,#buf)
	return 0
end

function time( ... )
	print("time")
	return true
end

function MessageHandle(name,connectid,iscont,buffer)
	local messageid, buf = package.PackageExplain(buffer)
	print(name,connectid,iscont,messageid, buf)
	protohandle.ProtoHandle(name,connectid,iscont,messageid,buf)
end

