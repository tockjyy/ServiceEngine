--package.path = package.path..';?.lua'
--print(package.path)
local class = require("class")
print("lua is run")

function Start()
	print("start is run")
	ret = StartNewListen("YY",7777)
	print ("ret1:"..tostring(ret))
	if ret then
		ret2 = StartNetWork()
		print("ret2:"..tostring(ret2))
	end
end

function MessageHandle(svrname,connectid,iscon,buffer)
	print(svrname,connectid,iscon,buffer)
	return svrname,connectid,iscon,buffer
end
slist = {}
slist[1] = class:new(100)
slist[2] = class:new(200)
slist[3] = class:new(300)
slist[4] = class:new(400)
for i,v in pairs(slist)
 do v:func() end

map = {}
map["one"] = 1
map["two"] = 2
map["three"] = 3

ma = {}
ma[1] = "one"
ma[2] = "two"

print(map["one"],map["two"],map["three"],ma[1])

maps = {8,1,2,3,4,5,6}
for i,v in pairs(map)
 do print(i,v) end
