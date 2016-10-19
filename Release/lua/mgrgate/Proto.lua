local protobuf = require "protobuf"

gate_mgrgate_proto = {}

protobuf.register_file("./lua/proto/gate_mgrgate.proto.pb")

gate_mgrgate_proto["REGGATE"] = protobuf.enum_id("gate_mgrgate.Protocol","REGGATE")
gate_mgrgate_proto["BEAT"] = protobuf.enum_id("gate_mgrgate.Protocol","BEAT")


proto = {}

function proto.encode(...)
	return protobuf.encode(...)
end

function proto.decode(...)
	return protobuf.decode(...)
end

return proto

--[[
protobuf.register_file("./lua/proto/addressbook.proto.pb")

messageid = {id = "REGGATE"}

print(protobuf.enum_id("gate_mgrgate.Protocol","REGGATE"))

code = protobuf.encode("gate_mgrgate.Proto", messageid)

decode = protobuf.decode("gate_mgrgate.Proto" , code)

print(decode.id)


--print(decode)



protobuf.register_file("./lua/proto/addressbook.proto.pb")


addressbook = {
	name = "Alice",
	id = 12345,
	phone = {
		{ number = "1301234567" },
		{ number = "87654321", type = "WORK" },
	}
}

code = protobuf.encode("tutorial.Person", addressbook)

decode = protobuf.decode("tutorial.Person" , code)

print(decode.name)
print(decode.id)
for _,v in ipairs(decode.phone) do
	print("\t"..v.number, v.type)
end

phonebuf = protobuf.pack("tutorial.Person.PhoneNumber number","87654321")
buffer = protobuf.pack("tutorial.Person name id phone", "Alice", 123, { phonebuf })
print(protobuf.unpack("tutorial.Person name id phone", buffer))
]]