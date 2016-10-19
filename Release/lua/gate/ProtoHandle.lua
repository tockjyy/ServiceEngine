local proto = require("Proto")
local package = require("Package")
local cpp = require("cpp_socket")

local protohandle = {}

function protohandle.ProtoHandle(name,connectid,iscont,messageid,buffer)
	if name == "MgrGate_Gate" then
		if messageid == gate_mgrgate_proto["REGGATE"] then --动态注册一个网关服务器
			local code = proto.decode("gate_mgrgate.RegGateRes", buffer)
			print("name:",name,"connectid:",connectid,"ret:",code.ret,"id:",code.id)
		elseif messageid == gate_mgrgate_proto["BEAT"] then --心跳
			local res ={
				pressure = 100
			}
			local buf = proto.encode("gate_mgrgate.BeatRes", res)
			cpp.SendMessage("MgrGate_Gate",0,true,package.PackageProduct(gate_mgrgate_proto["BEAT"],buf))
		end
	elseif name == "Client_MgrGate" then
		return
	else
		return
	end
end

return protohandle