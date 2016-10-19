local package = require("Package")
local proto = require "Proto"
local lock = require("cpp_lock")
local cpp = require("cpp_socket")

autoid = 1
local GateList = {}
List = {}
glock = lock.NewRWLock()

local Gate = {}

function Gate:new(connectid)
	o = {}
	self = {}
	setmetatable(o, self)
	self.__index = self
	self.id = autoid
	autoid = autoid + 1
	self.die = true
	self.pressure = 0
	self.ip = cpp.GetRemoveIp("MgrGate_Gate", connectid)
	return o
end

function GateSize()
	lock.ReadLock(glock)
	local count = 0
	for i,v in pairs(List) do
		count = count + 1
	end
	lock.UnReadLock(glock)
	return count
end

function GateList.BeatTimer()
	lock.WriteLock(glock)
	for i,v in pairs(List) do
		if v.die == false then
			List[i] = nil
			print("destory connect:",i)
		else
			cpp.SendMessage("MgrGate_Gate",i,false,package.PackageProduct(gate_mgrgate_proto["BEAT"],""))
			v.die = false
		end
	end
	lock.UnWriteLock(glock)
end

function GateList.RegGate(name,connectid)
	lock.WriteLock(glock)
	if List[connectid] == nil then
		List[connectid] = Gate:new(connectid)
	end
	lock.UnWriteLock(glock)
	print("--------------Connect Count:",GateSize())
	lock.ReadLock(glock)
	for i,v in pairs(List) do
		print("connectid:",i," Id:",v.id," Ip:",v.ip," pressure:",v.pressure)
	end
	lock.UnReadLock(glock)
	local res = {
		ret = 1,
		id = List[connectid].id
	}
	local buffer = proto.encode("gate_mgrgate.RegGateRes", res)
	cpp.SendMessage(name,connectid,false,package.PackageProduct(gate_mgrgate_proto["REGGATE"],buffer))
end


function GateList.BeatHandle(connectid,buffer)
	lock.ReadLock(glock)
	if List[connectid] ~= nil then
		List[connectid].die = true
		local code = proto.decode("gate_mgrgate.BeatRes",buffer)
		List[connectid].pressure = code.pressure
		--print(code.pressure)
	end
	lock.UnReadLock(glock)
end

return GateList
