package.path = package.path..';.\\lua\\mgrgate\\?.lua'..';.\\lua\\proto\\?.lua'..';.\\lua\\protobuf\\?.lua'
local config = require("ServiceConfig")
local package = require("Package")
local protohandle = require("ProtoHandle")
local proto = require("Proto")
local gate = require("MgrGate")
local cpp = require("cpp_socket")
require("Timers")

print("sha1: ",cpp.CreateSHA1("yy","123"))
function Start()

	if cpp.StartNewListen("MgrGate_Gate",ServicePort["MgrGate_Gate"],40) == false then
		print("MgrGate-Gate Start Error!")
	else
		print("MgrGate-Gate Start Sueecss!")
	end

	if cpp.StartNewListen("Client_MgrGate",ServicePort["Client_MgrGate"],30) == false then
		print("Client-MgrGate Start Error!")
	else
		print("Client-MgrGate Start Sueecss!")
	end
	if cpp.StartNetWork(8) == false then
		print("StartNetWork Error!")
	else
		print("StartNetWork Sueecss!")
	end

	return 0
end

function MessageHandle(name,connectid,iscont,buffer)
	local messageid, buf = package.PackageExplain(buffer)
	--print(name,connectid,iscont,messageid, buf)
	protohandle.ProtoHandle(name,connectid,iscont,messageid,buf)
end