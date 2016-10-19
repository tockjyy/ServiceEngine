# 代码说明
## 这里总共有两个工程,一个网关管理服务器和网关管理服务器的初步结构,包含了监听.连接.定时器.心跳.协议发送.读写锁的使用

##目录结构
### gate:网关服务器,客户端作用
### mgrgate:网关管理服务器,服务器作用
### proto:所有protobuf的协议定义文件与生成的pd文件生成文件夹
### protoc.exe:protobuf生成工具
### buildproto.bat:将所有proto下的.proto文件生成.pd文件的批处理文件
### Package.lua:将协议id和协议buf链接和拆分的小函数
### protobuf.lua:云风的pbc里提供的protobuf协议处理方法
### ServiceConfig.lua:配置文件
