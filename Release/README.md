//增加一个新的网络监听
name:网络名称
port:监听端口
ms:心跳时长(在此时间内未接收到心跳则释放连接)
bool cpp_socket.StartNewListen(name,port,ms)

//增加一个新的网络连接
name:网络名称
port:连接端口
ip:连接ip
bool cpp_socket.StartNewConnect(name,port,ip)

//向网络发送一组报文
name:网络名称
connectid:用于接收的会话id,在iscont为监听时有效
iscont:是连接还是监听 true连接 false监听
buffer:报文体
bool cpp_socket.SendMessage(name,connectid,iscont,buffer)

//服务器启动
cpu:工作线程的个数
bool cpp_socket.StartNetWork(cpu)

//获取连接的ip地址
name:网络名称
connectid:用于接收的会话id
string cpp_socket.GetRemoveIp(name,connectid)

//消息接收回调,要写在lua全局变量里面
name:网络名称
connectid:来自与哪个会话id的消息,在iscont为监听时有效
iscont:是连接还是监听 true连接 false监听
buffer:报文体
void MessageHandle(name,connectid,iscont,buffer)
//主函数回调,要写在lua全局变量里面
void Start()


uint NewSpinLock()
DoSpinLock(plock)
UnSpinLock(plock)


uint NewRWLock()
ReadLock(plock)
UnReadLock(plock)
WriteLock(plock)
UnWriteLock(plock)
DestorySpinLock(plock)
DestoryRWLock(plock)


bool NewTimer(func,index,ms)
bool NewOnceTimer(func,index,ms)
bool func(index)

{ "SetAESKey", SetAESKey },
{ "AESDeCrypt" , AESDeCrypt },
{ "AESEnCrypt" , AESEnCrypt },