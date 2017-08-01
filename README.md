# ServiceEngine
## 一.架构设想
这是本人在工作经验尚差时，于工作需要在进行语言选型时的尝试产物，仅有参考价值，其中的手法与库引用都显得稚嫩，之后的话都是我1年前的理解，估计下次有需要我才会整理。
理论上可作为任何Cpu密集型服务器的底层框架,网络底层由C++编写开发时可不变,服务器逻辑全部由Lua编写,节约开发成本与维护难度,库选型都是跨平台的库,所以只上传了VS2015的工程,linux编译未整理，不过很简单，没什么库差异

[详细设计笔记](https://github.com/tockjyy/ServiceEngine/wiki/%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E9%9A%8F%E7%AC%94)
### 例子 https://github.com/tockjyy/ServiceEngine/tree/master/Release
## 二.库组成
1.Boost/Asio库(1.60)(库过大,请自行安装编译)

2.Lua5.3(文件夹lua_vc14为window版lua api)

3.pbc(云风编写的,支持lua53与Protobuf的SDK)
## 三.技术组成
1.TCP监听与连接,连接池,分包与粘包,心跳连接清理机制

2.多线程支持,定时器支持,线程锁支持(包含读写锁与自旋锁)

3.加密支持,暂时只包含AES-128/SHA1
## 四.目录结构分析
1.Go --用Golang写了个相同架构的服务器,好玩的,有兴趣可以玩玩

2.lua_vc14 --window版lua api

3.pb --云风编写的pbc的源码,已经集成进工程,供参考

4.ServiceEngine --C++底层源码

5.Release -- C++编译好的框架,以及一个简单的lua服务器实例（如果没有安装VS2015请安装VC2015运行库）

QQ:305935690 逼逼了这么多真是不好意思,不合理之处请见谅,欢迎大牛指导



