local proto = require("Proto")
local gate = require("MgrGate")

local protohandle = {}

function protohandle.ProtoHandle(name,connectid,iscont,messageid,buffer)
	-- print(name,connectid,iscont,messageid,buffer)
	if name == "MgrGate_Gate" then
		if messageid == gate_mgrgate_proto["REGGATE"] then --动态注册一个网关服务器
			gate.RegGate(name,connectid)
		elseif messageid == gate_mgrgate_proto["BEAT"] then   --心跳
			gate.BeatHandle(connectid,buffer)
		end
	elseif name == "Client_MgrGate" then
		return
	else
		return
	end
end

return protohandle