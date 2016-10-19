local protobuf = require "protobuf"

gate_mgrgate_proto = {}
client_gate_proto = {}

protobuf.register_file("./lua/proto/gate_mgrgate.proto.pb")
protobuf.register_file("./lua/proto/client_gate.proto.pb")

gate_mgrgate_proto["REGGATE"] = protobuf.enum_id("gate_mgrgate.Protocol","REGGATE")
gate_mgrgate_proto["BEAT"] = protobuf.enum_id("gate_mgrgate.Protocol","BEAT")

client_gate_proto["LOGON"] = protobuf.enum_id("client_gate.Protocol","LOGON")
client_gate_proto["LOGIN"] = protobuf.enum_id("client_gate.Protocol","LOGIN")
client_gate_proto["BEAT"] = protobuf.enum_id("client_gate.Protocol","BEAT")


proto = {}

function proto.encode(...)
	return protobuf.encode(...)
end

function proto.decode(...)
	return protobuf.decode(...)
end

return proto