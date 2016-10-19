# 一.C++提供的Lua库
## 1.cpp_socket
### 1. 1. 增加一个新的网络监听
bool cpp_socket.StartNewListen(name,port,ms)

参数: name:网络名称  port:监听端口 ms:心跳时长(在此时间内,未发送出任何报文信息,则释放连接)

返回值: 是否成功
### 1. 2. 增加一个新的网络连接
bool cpp_socket.StartNewConnect(name,port,ip)

参数: name:网络名称 port:连接端口 ip:连接ip

返回值: 是否成功
### 1. 3. 向网络发送一组报文
bool cpp_socket.SendMessage(name,connectid,iscont,buffer)

参数: name:网络名称 connectid:用于接收的会话id,在iscont为监听时有效 iscont:是连接还是监听 buffer:报文体

返回值: 是否成功

### 1. 4. 服务器启动
bool cpp_socket.StartNetWork(cpu)

参数: cpu:工作线程的个数

返回值: 是否成功

### 1. 5. 获取连接的ip地址
string cpp_socket.GetRemoveIp(name,connectid)

参数: name:网络名称 connectid:用于接收的会话id

返回值: ipaddress

### 1. 6. 设置AES-128加密的秘钥
void cpp_socket.SetAESKey(key)

参数: key:16位秘钥

返回值: 无

### 1. 7. AES-128加密
string cpp_socket.AESEnCrypt(str)

参数: str:需要加密的数据

返回值: 加密后的数据

### 1. 8. AES-128解密
string cpp_socket.AESDeCrypt(str)

参数: str:需要解密的数据

返回值: 解密后的数据

### 1. 9. SHA1加密
string cpp_socket.CreateSHA1(str)

参数: str:需要加密的数据

返回值: 加密后的数据
## 2.cpp_lock
### 2. 1.创建一个新的自旋锁
uint cpp_lock.NewSpinLock()

返回值: 新建的自旋锁指针地址

### 2. 2.销毁一个自旋锁
void cpp_lock.DestorySpinLock(plock)

参数: 自旋锁指针地址

### 2. 3.锁住自旋锁
void cpp_lock.DoSpinLock(plock)

参数: 自旋锁指针地址

### 2. 4.解锁自旋锁
void cpp_lock.UnSpinLock(plock)

参数: 自旋锁指针地址

### 2. 5.创建一个新的读写锁
uint cpp_lock.NewRWLock()

返回值: 新建的自旋锁指针地址

### 2. 6.销毁一个读写锁
void cpp_lock.DestoryRWLock(plock)

参数: 自旋锁指针地址

### 2. 7.锁住读锁
void cpp_lock.ReadLock(plock)

参数: 自旋锁指针地址

### 2. 8.解锁读锁
void cpp_lock.UnReadLock(plock)

参数: 自旋锁指针地址

### 2. 9.锁住写锁
void cpp_lock.WriteLock(plock)

参数: 自旋锁指针地址

### 2. 10.解锁写锁
void cpp_lock.UnWriteLock(plock)

参数: 自旋锁指针地址

## 3.cpp_timer
### 3. 1.新建一个定时器
bool cpp_timer.NewTimer(func,index,ms)

参数: func:定时回调函数(请用lua编写且位于全局变量) index:回调函数参数 ms:延时(单位:ms)

返回值: 是否成功

### 3. 2.新建一个一次性定时器
bool cpp_timer.NewOnceTimer(func,index,ms)

参数: func:定时回调函数(请用lua编写且位于全局变量) index:回调函数参数 ms:延时(单位:ms)

返回值: 是否成功

### 3. 3.定时回调函数
bool func(index)(名称必须注册时的名称相同)

参数: index:一般用于识别激发这个定时器的角色

返回值: 是否成功,如果返回false将销毁这个定时器


# 二.Lua中必须编写的入口回调函数(位于lua全局变量)
## 1.主函数回调
### function Start() 无参数无返回值,程序启动时调用
## 2.消息接收回调
### void MessageHandle(name,connectid,iscont,buffer) 当有网络报文到达时被C++调用
name:网络名称

connectid:来自与哪个会话id的消息,在iscont为监听时有效

iscont:是连接还是监听 true连接 false监听

buffer:报文体
