Package = {}

function Package.PackageExplain(buffer)
	local buf = string.sub(buffer, 3, -1)
	local num = string.byte(buffer,1)
	local highnum = string.byte(buffer,2)
	local messageid = highnum*256 + num
	return messageid,buf
end

function Package.PackageProduct(messageid,buffer)
	local highnum = math.floor(messageid/256)
	local num = messageid%256
	return string.format("%c", num)..string.format("%c", highnum)..buffer
end

return Package