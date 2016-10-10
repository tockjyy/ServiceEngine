print("class open sucess")
require("student")

local class = {}

function class:new(num)
local self = {}--创建新的表作为实例的对象
self.test = num
setmetatable( self , {__index = class})  --设置class为对象元表的__index
return self     --返回该新表
end

function class:func()
print("class : func",self.test)
end

return class