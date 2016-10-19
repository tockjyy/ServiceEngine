local package = require("Package")
local proto = require "Proto"

MgrClient = {}

ClientList = {}
glock = NewRWLock()
local Client {}

function Gate:new(connectid,userid)
	o = {}
	self = {}
	setmetatable(o, self)
	self.__index = self
	self.id = userid
	self.die = true
	self.ip = GetRemoveIp("Client_Gate", connectid)
	return o
end

function ClientSize()
	ReadLock(glock)
	local count = 0
	for i,v in pairs(ClientList) do
		count = count + 1
	end
	UnReadLock(glock)
	return count
end

function MgrClient.NewClient(connectid,userid)
	WriteLock(glock)
	ClientList[connectid] = Gate:new(connectid,userid)
	UnWriteLock(glock)
end

function MgrClient.BeatTimer()
	WriteLock(glock)
	for i,v in pairs(ClientList) do
		if v.die == false then
			List[i] = nil
		else
			SendMessage("Client_Gate",i,false,package.PackageProduct(client_gate_proto["BEAT"],""))
			v.die = false
		end
	end
	UnWriteLock(glock)
end

return MgrClient